# Song-Display
Display current playing song parsing from Media Transport Control

![Screenshot_2](https://user-images.githubusercontent.com/95361171/227369134-5d6a7d90-29d5-428f-9cb7-a7148c533123.png)

![Screenshot_1](https://user-images.githubusercontent.com/95361171/227368956-4bc11353-f48d-4e0b-b35f-cca686fe931e.png)

```
static bool Initialized = false;
if (!Initialized) {
    player.Initialize();
    Initialized = true;
}

player.Update(g_pd3dDevice);

ImGui::Begin("Window");
{
    if (player.HasMedia)
    {
        ImGui::Text(player.Title);
        ImGui::Text(player.Artist);
        if (player.thumb != nullptr)
            ImGui::Image(player.thumb, ImVec2(30, 30));
    }
}
ImGui::End();
 ```
