#include "titlemenu.h"
#include "../profile_internal.h"
#include "../games/rne/tilebackground.h"
#include "../games/rne/titlemenu.h"
#include "../games/dash/titlemenu.h"
#include "../games/chlcc/titlemenu.h"
#include "../games/mo6tw/titlemenu.h"
#include "../../log.h"
#include "../../window.h"

namespace Impacto {
namespace Profile {
namespace TitleMenu {

using namespace Impacto::TitleMenu;

TitleMenuType Type = TitleMenuType::None;

Sprite PressToStartSprite;
Sprite MenuEntriesSprites[MenuEntriesNumMax];
Sprite MenuEntriesHSprites[MenuEntriesNumMax];

int MenuEntriesNum;

float PressToStartAnimDurationIn;
float PressToStartAnimDurationOut;

float PressToStartX;
float PressToStartY;

static void GetMemberSpriteArray(Sprite* arr, uint32_t count,
                                 char const* name) {
  EnsurePushMemberOfType(name, kArrayType);

  if (TopVal().Size() != count) {
    ImpLog(LL_Fatal, LC_Profile, "Expected to have %d sprites for %s\n", count,
           name);
    Window::Shutdown();
  }

  for (uint32_t i = 0; i < count; i++) {
    arr[i] = EnsureGetArrayElementSprite(i);
  }

  Pop();
}

void Configure() {
  if (TryPushMember("TitleMenu")) {
    AssertIs(kObjectType);

    Type = TitleMenuType::_from_integral_unchecked(EnsureGetMemberInt("Type"));

    if (Type == +TitleMenuType::RNE) {
      Implementation = RNE::TitleMenu::Configure();
    } else if (Type == +TitleMenuType::Dash) {
      Implementation = Dash::TitleMenu::Configure();
    } else if (Type == +TitleMenuType::CHLCC) {
      Implementation = CHLCC::TitleMenu::Configure();
    } else if (Type == +TitleMenuType::MO6TW) {
      Implementation = MO6TW::TitleMenu::Configure();
    }

    MenuEntriesNum = EnsureGetMemberInt("MenuEntriesNum");
    if (MenuEntriesNum > 0) {
      GetMemberSpriteArray(MenuEntriesSprites, MenuEntriesNum,
                           "MenuEntriesSprites");
      GetMemberSpriteArray(MenuEntriesHSprites, MenuEntriesNum,
                           "MenuEntriesHighlightedSprites");
    }

    PressToStartSprite = EnsureGetMemberSprite("PressToStartSprite");

    PressToStartAnimDurationIn =
        EnsureGetMemberFloat("PressToStartAnimDurationIn");
    PressToStartAnimDurationOut =
        EnsureGetMemberFloat("PressToStartAnimDurationOut");

    Implementation->PressToStartAnimation.DurationIn =
        PressToStartAnimDurationIn;
    Implementation->PressToStartAnimation.DurationOut =
        PressToStartAnimDurationOut;
    Implementation->PressToStartAnimation.LoopMode = ALM_ReverseDirection;

    PressToStartX = EnsureGetMemberFloat("PressToStartX");
    PressToStartY = EnsureGetMemberFloat("PressToStartY");
    Pop();
  }
}

}  // namespace TitleMenu
}  // namespace Profile
}  // namespace Impacto