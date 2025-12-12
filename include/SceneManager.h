#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

enum SceneType
{
    SCENE_MENU,
    SCENE_PLAY,
    SCENE_GAMEOVER
};

class SceneManager
{
private:
    SceneType currentScene;
    SceneType nextScene;

public:
    SceneManager();

    void setScene(SceneType scene);
    SceneType getCurrentScene() const { return currentScene; }
    SceneType getNextScene() const { return nextScene; }
    void updateScene();
    bool hasSceneChanged() const;

private:
    void transitionScene();
};

#endif