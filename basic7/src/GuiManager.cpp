#include "GuiManager.h"

GuiManager::GuiManager() :
    mExitButtonPressed(false)
{}

void GuiManager::pressExitButton() {
    mExitButtonPressed = true;
}

bool GuiManager::isExitButtonPressed() {
    return mExitButtonPressed;
}