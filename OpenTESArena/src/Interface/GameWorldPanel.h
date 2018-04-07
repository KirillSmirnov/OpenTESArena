#ifndef GAME_WORLD_PANEL_H
#define GAME_WORLD_PANEL_H

#include <array>
#include <vector>

#include "Button.h"
#include "Panel.h"
#include "TextBox.h"
#include "../Math/Rect.h"

// When the GameWorldPanel is active, the game world is ticking.

// There are two desired kinds of interfaces:
// - The original: compass, portrait, stat bars, and buttons with original mouse.
// - A modern version: only compass and stat bars with free-look mouse.

class Player;
class Renderer;
class TextureManager;

class GameWorldPanel : public Panel
{
private:
	std::unique_ptr<TextBox> playerNameTextBox;
	Button<Game&> characterSheetButton, statusButton,
		logbookButton, pauseButton;
	Button<Player&> drawWeaponButton;
	Button<> stealButton, magicButton, useItemButton, campButton;
	Button<GameWorldPanel&> scrollUpButton, scrollDownButton;
	Button<Game&, bool> mapButton;
	std::array<Rect, 9> nativeCursorRegions;
	std::vector<Int2> weaponOffsets;

	// Modifies the values in the native cursor regions array so rectangles in
	// the current window correctly represent regions for different arrow cursors.
	void updateCursorRegions(int width, int height);

	// Handles input for the player camera.
	void handlePlayerTurning(double dt, const Int2 &mouseDelta);

	// Handles input for player movement in the game world.
	void handlePlayerMovement(double dt);

	// Handles input for the player's attack. Takes the change in mouse position since
	// the previous frame.
	void handlePlayerAttack(const Int2 &mouseDelta);

	// Sends an "on voxel enter" message for the given voxel and triggers any text or
	// sound events.
	void handleTriggers(const Int2 &voxel);

	// Checks the given voxel to see if it's a transition voxel (i.e., level up/down),
	// and changes the current level if it is.
	void handleLevelTransition(const Int2 &playerVoxel, const Int2 &transitionVoxel);

	// Draws a tooltip sitting on the top left of the game interface.
	void drawTooltip(const std::string &text, Renderer &renderer);

	// Draws the compass for some given player direction in the XZ plane.
	void drawCompass(const Double2 &direction, TextureManager &textureManager,
		Renderer &renderer);

	// Draws some debug text.
	void drawDebugText(Renderer &renderer);
public:
	// Constructs the game world panel. The GameData object in Game must be
	// initialized.
	GameWorldPanel(Game &game);
	virtual ~GameWorldPanel() = default;

	virtual std::pair<SDL_Texture*, CursorAlignment> getCurrentCursor() const override;
	virtual void handleEvent(const SDL_Event &e) override;
	virtual void resize(int windowWidth, int windowHeight) override;
	virtual void tick(double dt) override;
	virtual void render(Renderer &renderer) override;
	virtual void renderSecondary(Renderer &renderer) override;
};

#endif
