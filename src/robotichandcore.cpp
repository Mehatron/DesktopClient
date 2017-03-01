#include "robotichandcore.h"

bool operator==(const RoboticHandCore::State &lhs, const RoboticHandCore::State &rhs)
{
    if(lhs.mode != rhs.mode ||
        lhs.constructionDown != rhs.constructionDown ||
        lhs.constructionUp != rhs.constructionUp ||
        lhs.left != rhs.left ||
        lhs.right != rhs.right ||
        lhs.rotationDown != rhs.rotationDown ||
        lhs.rotationUp != rhs.rotationUp ||
        lhs.extendsUnextended != rhs.extendsUnextended ||
        lhs.extendsExtended != rhs.extendsExtended ||
        lhs.picked != rhs.picked)
        return false;

    return true;
}
