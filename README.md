# Music display
Display current song analysis in Media Transport Control using dx11

![Screenshot_1](https://github.com/user-attachments/assets/2a6277fd-4d44-49d0-b3dd-fcda0d4edf83)

![Screenshot_2](https://github.com/user-attachments/assets/a36ffb0a-fedd-44d6-820c-3db80b0153e3)

```
    static bool Initialized = false;
    if (!Initialized) {
        player.Initialize();
        Initialized = true;
    }

    player.Update(g_pd3dDevice, g_pd3dDeviceContext);

    static std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();
    static float accumulatedTime = 0.0f;
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    accumulatedTime += deltaTime.count();

    ImGui::SetNextWindowSize(ImVec2(500.0f, 260.0f));
    ImGui::Begin("Now Playing", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (player.HasMedia)
    {
        float elapsedSeconds = player.CurrentTime + accumulatedTime;

        accumulatedTime = 0.0f;

        ImVec2 thumbnailSize(150.0f, 150.0f);
        ImGui::SameLine();
        if (player.thumb != nullptr)
        {
            float newXPos = ImGui::GetWindowWidth() - thumbnailSize.x - ImGui::GetStyle().WindowPadding.x - 350.0f;
            ImGui::SetCursorPosX(newXPos);
            ImGui::Image((void*)player.thumb, thumbnailSize);
        }

        ImGui::BeginGroup();

        std::string title = WStringToString(player.Title);
        std::string artist = WStringToString(player.Artist);

        if (!title.empty() && !artist.empty()) {
            ImGui::Text("%s", title.c_str());
            ImGui::Text("%s", artist.c_str());
        }
        else {
            ImGui::Text("Title or Artist not available");
        }
        ImGui::EndGroup();

        float progress = (player.TotalTime > 0) ? elapsedSeconds / player.TotalTime : 0.0f;

        int elapsedMinutes = static_cast<int>(elapsedSeconds) / 60;
        int elapsedRemSeconds = static_cast<int>(elapsedSeconds) % 60;
        int totalMinutes = player.TotalTime / 60;
        int totalSeconds = player.TotalTime % 60;

        ImGui::Text("%d:%02d", elapsedMinutes, elapsedRemSeconds);
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 210.0f);
        ImGui::Text("%d:%02d", totalMinutes, totalSeconds);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float barWidth = ImGui::GetWindowWidth() - thumbnailSize.x - 40.0f;
        float barHeight = 4.0f;
        float filledWidth = barWidth * progress;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + barWidth, pos.y + barHeight), IM_COL32(100, 100, 100, 255));
        draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + filledWidth, pos.y + barHeight), IM_COL32(255, 255, 255, 255));

        ImGui::Dummy(ImVec2(0.0f, barHeight + 2.0f));
    }
    else
    {
        ImGui::Text("No media playing");
    }
 ```
