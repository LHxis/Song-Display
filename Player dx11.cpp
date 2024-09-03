#include "player.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <winrt/Windows.Foundation.h>
#include <vector>
#include <imgui.h>

Player player;
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;

void Player::Initialize()
{
    this->sessionManager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
}

std::string WStringToString(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &str[0], size_needed, nullptr, nullptr);
    return str;
}

void CreateTextureFromBytes(const std::vector<unsigned char>& imageBytes, ID3D11Device* device, ID3D11ShaderResourceView** outTexture)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load_from_memory(imageBytes.data(), imageBytes.size(), &width, &height, &channels, STBI_rgb_alpha);
    if (!imageData)
    {
        std::cerr << "Failed to load image from memory." << std::endl;
        return;
    }

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = imageData;
    initData.SysMemPitch = width * 4;

    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = device->CreateTexture2D(&desc, &initData, &texture);
    if (SUCCEEDED(hr))
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = desc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        hr = device->CreateShaderResourceView(texture, &srvDesc, outTexture);
        texture->Release();
    }

    if (FAILED(hr))
    {
        std::cerr << "Failed to create texture from memory. HRESULT: " << hr << std::endl;
    }

    stbi_image_free(imageData);
}

void* LastThumb = nullptr;

concurrency::task<void> Player::Update(ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pd3dDeviceContext)
{
    try
    {
        this->session = this->sessionManager->GetCurrentSession();

        if (this->session != nullptr)
        {
            this->HasMedia = true;

            auto info = co_await this->session->TryGetMediaPropertiesAsync();
            this->Title = info.Title();
            this->Artist = info.Artist();
            this->AlbumArtist = info.AlbumArtist();
            this->AlbumTitle = info.AlbumTitle();
            this->TrackNumber = info.TrackNumber();
            this->AlbumTrackCount = info.AlbumTrackCount();

            if (info.Thumbnail())
            {

                auto thumbnail_stream = co_await info.Thumbnail().OpenReadAsync();
                this->Thumbnail_type = thumbnail_stream.ContentType();
                Buffer buffer = Buffer(thumbnail_stream.Size());
                co_await thumbnail_stream.ReadAsync(buffer, buffer.Capacity(), InputStreamOptions::ReadAhead);
               
                this->Thumbnail_type = thumbnail_stream.ContentType();
                if (this->Thumbnail_buffer)
                {
                    free(this->Thumbnail_buffer);
                }

                this->Thumbnail_buffer = malloc(buffer.Length());
                if (!this->Thumbnail_buffer)
                {
                    std::cerr << "Failed to allocate memory for thumbnail buffer." << std::endl;
                    co_return;
                }
                memcpy(this->Thumbnail_buffer, buffer.data(), buffer.Length());
                this->Thumbnail_size = buffer.Length();

                if (this->Thumbnail_buffer != LastThumb)
                {
                    std::vector<unsigned char> imageBytes(static_cast<unsigned char*>(this->Thumbnail_buffer), static_cast<unsigned char*>(this->Thumbnail_buffer) + this->Thumbnail_size);

                    CreateTextureFromBytes(imageBytes, g_pd3dDevice, &this->thumb);

                    LastThumb = this->Thumbnail_buffer;
                }
            }
            else
            {
                this->Thumbnail_size = 0;
                this->thumb = nullptr;
            }

            auto timelineProperties = this->session->GetTimelineProperties();
            this->TotalTime = static_cast<int>(timelineProperties.EndTime().count() / 10000000);
            this->CurrentTime = static_cast<int>(timelineProperties.Position().count() / 10000000);
        }
        else
        {
            this->HasMedia = false;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error in Player Update: " << e.what() << std::endl;
    }
}
