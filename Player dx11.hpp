#pragma once

#pragma comment(lib, "windowsapp")

#include <winrt/base.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <ppltasks.h>
#include <pplawait.h>
#include <sdkddkver.h>

#include <chrono>
#include <functional>
#include <sstream>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <iostream>
#pragma comment(lib, "d3d11.lib")

using namespace winrt;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Media::Control;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation::Collections;
using namespace winrt::Windows::Media::Control;
using namespace DirectX;

// Inclua o namespace da fundação para corrotinas
using namespace winrt::Windows::Foundation;
std::string WStringToString(const std::wstring& wstr);
class Player
{
private:
    std::optional<GlobalSystemMediaTransportControlsSessionManager> sessionManager;
    std::optional<IRandomAccessStreamWithContentType> thumbnail;
public:
    std::optional<GlobalSystemMediaTransportControlsSession> session;
    bool HasMedia = false;
    std::wstring SourceAppUserModeId;
    std::wstring Title;
    std::wstring Artist;
    std::wstring AlbumArtist;
    std::wstring AlbumTitle;
    int TrackNumber;
    int AlbumTrackCount;
    std::wstring Thumbnail_type;
    void* Thumbnail_buffer;
    int Thumbnail_size;
    bool hasThumbnail = false;
    int TotalTime;
    int CurrentTime;

    ID3D11ShaderResourceView* thumb;

    void Initialize();
    concurrency::task<void> Update(ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pd3dDeviceContext);
};

extern Player player;
