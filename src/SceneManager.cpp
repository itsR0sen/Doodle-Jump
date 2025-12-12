#include "SceneManager.h"

SceneManager::SceneManager()
    : currentScene(SCENE_MENU), nextScene(SCENE_MENU)
{
}

void SceneManager::setScene(SceneType scene)
{
    nextScene = scene;
}

void SceneManager::updateScene()
{
    if (currentScene != nextScene)
    {
        transitionScene();
    }
}

bool SceneManager::hasSceneChanged() const
{
    return currentScene != nextScene;
}

void SceneManager::transitionScene()
{
    currentScene = nextScene;
}