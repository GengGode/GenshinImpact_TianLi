#pragma once
#include "../Capture.h"

#include <windows.h>

#include <Unknwn.h>
#include <inspectable.h>

#include <winrt/Windows.Graphics.Capture.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include <winrt/Windows.UI.Popups.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/windows.graphics.directx.h>
#include <winrt/windows.graphics.directx.direct3d11.h>
#include <winrt/windows.foundation.metadata.h>

#include <winrt/Windows.Graphics.Capture.h>
#include <windows.graphics.capture.interop.h>
#include <windows.graphics.capture.h>

#include <d3d11.h>
//#include <dxgi.h>
//#include <d3d11_4.h>

class DirectXCapture : public Capture
{
    winrt::Windows::Graphics::Capture::GraphicsCaptureItem item{ nullptr };
    winrt::Windows::Graphics::Capture::GraphicsCaptureSession session{ nullptr };
    winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool frame_pool{ nullptr };
    winrt::Windows::Graphics::Capture::Direct3D11CaptureFrame frame{ nullptr };
    winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice device{ nullptr };
    winrt::com_ptr<ID3D11Texture2D> texture{ nullptr };
    winrt::com_ptr<ID3D11Device> d3d_device{ nullptr };
    winrt::com_ptr<IDXGIDevice> dxgi_device{ nullptr };

    winrt::com_ptr<ID3D11DeviceContext> d3d_context{ nullptr };
public:
    DirectXCapture();
    ~DirectXCapture() override;
public:
    bool init() override;
    bool uninit() override;
    bool capture(cv::Mat& frame) override;
    bool setHandle(HWND handle = 0) override;
public:
    D3D11_TEXTURE2D_DESC desc_type{ 0,0,1,1,DXGI_FORMAT_B8G8R8A8_UNORM,{1,0},D3D11_USAGE_STAGING,0,D3D11_CPU_ACCESS_READ,0 }; 
    bool isInitialized = false;
    bool isBaseCapture = true;
};

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

extern "C"
{
    HRESULT __stdcall CreateDirect3D11DeviceFromDXGIDevice(::IDXGIDevice* dxgiDevice,
        ::IInspectable** graphicsDevice);
}

DirectXCapture::DirectXCapture() {
    type = TianLi::Track::CaptureType::DirectX;
    winrt::com_ptr<ID3D11Device> device;
    winrt::com_ptr<ID3D11DeviceContext> context;
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
    auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &feature_level, 1, D3D11_SDK_VERSION, device.put(), nullptr, context.put());
    if (DXGI_ERROR_UNSUPPORTED == hr) {
        hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &feature_level, 1, D3D11_SDK_VERSION, device.put(), nullptr, context.put());
    }
    winrt::check_hresult(hr);
    this->d3d_device = device;
    this->d3d_context = context;
    this->dxgi_device = device.as<IDXGIDevice>();
    winrt::com_ptr<::IInspectable> _device;
    winrt::check_hresult(CreateDirect3D11DeviceFromDXGIDevice(dxgi_device.get(), _device.put()));
    this->device = _device.as<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>();
}
DirectXCapture::~DirectXCapture() {
    uninit();
}
bool DirectXCapture::init() {
    if (isInitialized) {
        return true;
    }

    auto activation_factory = winrt::get_activation_factory<winrt::Windows::Graphics::Capture::GraphicsCaptureItem>();
    auto interop_factory = activation_factory.as<IGraphicsCaptureItemInterop>();
    interop_factory->CreateForWindow(genshin_handle, winrt::guid_of<ABI::Windows::Graphics::Capture::IGraphicsCaptureItem>(), reinterpret_cast<void**>(winrt::put_abi(this->item)));
    if (!item) {
        auto monitor_handle = MonitorFromWindow(genshin_handle, MONITOR_DEFAULTTONEAREST);
        interop_factory->CreateForMonitor(monitor_handle, winrt::guid_of<ABI::Windows::Graphics::Capture::IGraphicsCaptureItem>(), reinterpret_cast<void**>(winrt::put_abi(this->item)));
    }
    if (nullptr == this->item) {
        err = { 1,"CreateForWindow failed" };
        return false;
    }

    frame_pool = winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::Create(
        device,
        winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
        2,
        item.Size());
    session = frame_pool.CreateCaptureSession(item);

    // 判断 WindowsSDK 版本大于等于 10.0.22000.0
#if (WINVER >= _WIN32_WINNT_WIN10_21H1)
    try
    {
        if (winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Graphics.Capture.GraphicsCaptureSession", L"IsBorderRequired"))
        {
            winrt::Windows::Graphics::Capture::GraphicsCaptureAccess::
                RequestAccessAsync(
                    winrt::Windows::Graphics::Capture::
                    GraphicsCaptureAccessKind::Borderless)
                .get();
            session.IsBorderRequired(false);
            session.IsCursorCaptureEnabled(false);
        }
    }
    catch (...)
    {
        // Ignore any errors
    }
#endif

    session.StartCapture();
    isInitialized = true;
    return true;
}
bool DirectXCapture::uninit() {
    if (!isInitialized) {
        return true;
    }
    session.Close();
    session = nullptr;
    frame_pool.Close();
    frame_pool = nullptr;
    item = nullptr;
    isInitialized = false;
    return true;
}

#include <winrt/windows.graphics.directx.direct3d11.h>

struct __declspec(uuid("A9B3D012-3DF2-4EE3-B8D1-8695F457D3C1"))
    IDirect3DDxgiInterfaceAccess : ::IUnknown
{
    virtual HRESULT __stdcall GetInterface(GUID const& id, void** object) = 0;
};
bool DirectXCapture::capture(cv::Mat& frame)
{
    if (isInitialized == false) {
        init();
        err = { 1, "DirectShowCapture is not initialized" };
        return false;
    }
    auto que = frame_pool.DispatcherQueue();
	
    this->frame = frame_pool.TryGetNextFrame();

    if (nullptr == this->frame) {
        err = { 2, "DirectShowCapture get frame failed" };
        return false;
    }
    auto frame_size = this->frame.ContentSize();


    winrt::Windows::Foundation::IInspectable _surface = this->frame.Surface();
    auto access = _surface.as<IDirect3DDxgiInterfaceAccess>();
    winrt::com_ptr<ID3D11Texture2D> frameSurface;
    winrt::check_hresult(access->GetInterface(winrt::guid_of<ID3D11Texture2D>(), frameSurface.put_void()));

    D3D11_TEXTURE2D_DESC desc_type{ (unsigned int)frame_size.Width,(unsigned int)frame_size.Height,1,1,DXGI_FORMAT_B8G8R8A8_UNORM,{1,0},D3D11_USAGE_STAGING,0,D3D11_CPU_ACCESS_READ,0 };
    static ID3D11Texture2D* bufferTexture;

    d3d_device->CreateTexture2D(&desc_type, nullptr, &bufferTexture);
    d3d_context->CopyResource(bufferTexture, frameSurface.get());
    if (bufferTexture == nullptr) {
        err = { 3, "DirectShowCapture get bufferTexture failed" };
        return false;
    }
    D3D11_MAPPED_SUBRESOURCE mappedTex;
    d3d_context->Map(bufferTexture, 0, D3D11_MAP_READ, 0, &mappedTex);

    auto data = mappedTex.pData;
    auto pitch = mappedTex.RowPitch;

    // 将画面转换为OpenCV的Mat
    auto frame_mat = cv::Mat(frame_size.Height, frame_size.Width, CV_8UC4, (void*)data, pitch);
    // 释放资源
    bufferTexture->Release();
    frame = frame_mat;
    return true;
}

bool DirectXCapture::setHandle(HWND handle) {
    if (genshin_handle == handle) {
        return true;
    }
    this->genshin_handle = handle;
    if (isInitialized)
    {
        uninit();
        return init();
    }
    else
    {
        return true;
    }
}
