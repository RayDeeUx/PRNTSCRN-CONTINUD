# <cy>PRNTSCRN</c> Changelog
## v1.0.11
<c-aaaaaa>*Disclaimer: For a number of reasons (especially legal), any sub-bullet points referring to Update 2.2081 in this changelog should be taken with no less than 40 trillion kilograms of salt and 60 billion mana orbs (or your real-life local currency's equivalent value). These sub-bullet points only represents RayDeeUx's (Erymanthus') opinions on Update 2.2081, and no one else's. These sub-bullet points about Update 2.2081 should not be treated as fact or gospel, and should not be quoted out of context, as they were written during a state of deep frustration.*</c>
- Fix that darn bug where scrollable layers would be partially cropped out in screenshots taken by PRNTSCRN itself.
  - This bug would've been noticed and fixed sooner had it not been for a number of choices that others besides PRNTSCRN's contributors had made, primarily RobTop's approach with writing Update 2.2081 with the end result so poor in quality that it's worth wondering if someone else (perhaps a clanker) wrote the code responsible for Update 2.2081. But I digress (even if the low-quality code that came out of Update 2.2081 strongly influenced my personal decision to stay on Geode v4 and GD 2.2074).
  - There were also some smaller changes within Geode v5 itself that caused this issue, but it was a long-overdue (though quietly articulated) fix that would've happened one way or another. But I digress (again).
  - Point being that my decision to not update to Geode v5 and GD 2.2081 came back to bite me (as seen with the bug this update aims to fix), and I probably should've fixed this sooner despite these circumstances.
  - TODO: fill this line in with credit to whoever fixes the bug
## v1.0.10 and v1.0.9
- Restore Eclipse Menu integration.
  - That's the whole update.
## v1.0.8
- Port to Geode v5.0.0-beta.3 and GD 2.2081.
- Temporarily remove Eclipse Menu integration.
- Added a new option to hide Noclip Tints from most major mod menus during screenshot.
  - This is to compensate for a few node IDs this mod claimed to filter, only to never actually do anything to them.
  - These options might also prove useful in a number of niche scenarios not worth discussing here.
- Added nodes from Difficulty Progression and MegaHack v9 to the "Hide UI" filter toggle.
  - These changes are not going away any time soon, and if anything, should've been added as early as v1.0.5.
- Added the dummy trail from Editor Trail In-Game to the "Hide Player" filter toggle.
  - The editor trail added by this mod is tied to the player.
- Automatically hide the grayscale shader from GrayscaleMode by default.
  - Originally, you had to hide the grayscale shader on your own if you were to take a screenshot inside a level.
## v1.0.7
- Fix some UI problems. Sorry about that!
## v1.0.6
- Added options to hide the editor grid, editor playtest path, and editor object hitboxes.
  - These options were inspired by <c-ff007d>Au</c><c-fffa7f>sk</c>.
## v1.0.5
- Minor bugfixes related to the API. Hopefully.
## v1.0.4
- Minor bugfixes related to the API. Hopefully.
## v1.0.3
- Consistently hide checkpoint objects.
## v1.0.2
- Consistently hide UI elements.
## v1.0.1
- Added detection for texture quality changes.
## v1.0.0
- Initial release (on GitHub).