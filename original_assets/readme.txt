Moktar the Fox 2.1.1 by Jesses

What it is:
 Moktar the Fox is a level editor/viewer, code generator and trainer for
 Moktar and Titus the Fox.

Installation:
 Just place MTF.COM in your Moktar or Titus the Fox directory.

Command-line options:
 MTF [/?] [PLAY|COD|EDIT] [TRN] [/V|/E|/T|/C|/H] [/F] [/A|/B] [/W] [/R]

  /?	- Help
  PLAY	- Runs the game
  COD	- Displays the level codes
  EDIT	- Runs the viewer/editor
  TRN	- Enables the trainer, during play you can press:
	  F5 - Toggle unlimited health
	  F6 - Toggle unlimited lives
	  F7 - Toggle noclip mode: use arrows to move around
  /V	- VGA graphics
  /E	- EGA graphics
  /T	- Tandy graphics
  /C	- CGA graphics
  /H	- Hercules graphics
  /F	- Fading disabled (only effective with VGA, ignored by Moktar)
  /A	- AdLib sound, forced
  /B	- Beeper sound, forced
  /W	- Disable version warning
  /R	- Resolution in the editor changes from 640*480 to 800*600

Notes:
 - Do NOT overwrite LEVEL0.SQZ or another level file with the editor's save
   command! The format of the files it creates is identical to that of the
   SQZ files, but they are not compressed, while SQZ files are. This most
   likely will cause evil things to happen when you try to open the levels.
   Just save them to some other file; they can only be played via the level
   viewer's "P" command for now. Playing them in an easier way will be
   possible once a proper custom level file format is included in MTF.
 - The editor currently only saves everything that is stored in the LEVEL?.SQZ
   files. This means things like variable colors, sprites, level name and
   music cannot be changed yet.
 - A no-floppy patch is already included so you shouldn't need any other patch
 - If the program complains that it can't find an executable, you probably
   have a version of the games it doesn't know yet. To make it work, mail me
   (address below) and I'll add your version.

Quick usage:
	To play a custom level, go to the Viewer/Editor main menu (MTF EDIT),
	  press 1 (Level viewer/Editor), ENTER (edit), L (Load),
	  enter the level's file name, ESCAPE, P (play).

	All menus can be quit by pressing Escape. In the level viewer/editor,
	  the arrow keys can be used to move around; holding shift moves one
	  screen at a time, while not holding shift moves one tile/pixel.

	Initial menu:
		Displayed when none of PLAY, TRN, COD or EDIT is entered. The
		4 menu options can also be chosen by using this command line:
			1. MTF PLAY
			2. MTF TRN
			3. MTF COD
			4. MTF EDIT

	Trainer:
		Press F5, F6 and F7 to toggle unlimited health / lives and
		noclip mode

	Viewer/Editor main menu:
		The 8 options should speak for themselves

	Level viewer:
		The bottom row contains help; press the highlighted character
		to execute the accompanying action. A note on some of the
		actions:
			- Play: Starts the game and loads the currently
			  visible level. You can play your edited levels
			  without saving them first if you like. After you
			  finish the level, the level viewer will be reloaded.
			- Write BMP: does nothing yet
			- ENTER: starts the editor for this level
			- Toggles: To show hidden tiles, disable bonus (hit B)
	Editor:
		Again the bottom row explains all. Some notes:
			- This "Clear" resets the entire level (in contrast to
			  the other clear commands which only clear a part)
			- Load/Save: use this to manage your edited level
			- Tile editor: runs the tile editor
			- Size: alters the level's height. Can be from 12
			  tiles to 255 tiles
			- Copy/Paste: works with rectangles. The first time
			  "Y" is pressed, the first corner is set to the tile
			  cursor; the second time "Y" is pressed, the tiles
			  between the first corner and the current tile cursor
			  are copied. Paste then pastes this tile rectangle at
			  the tile cursor.
			- Placing things:
			  - Start/Exit: this is where you begin/end the level.
			    The exit will only work after all boss enemies
			    have been defeated
			  - Barrier: the game won't scroll to the right of and
			    below this barrier, unless the player actually
			    passes it. As soon as the player moves above the
			    barrier the game will scroll upwards; moving to
			    the left of it does not scroll immediately. Useful
			    to hide secret rooms like in level 3 and 7
			    (horizontal), and to hide what's below the floor
			    like the last part of level 11 (vertical)
			  - Bonus/Nmi/Obj/Elevator/Gate: these start one of 5
			    editors for placing things, see below for details.
			  - Tiles: In the tile editor, you can select up to 16
			    active tiles. Out of this set, one will be chosen
			    randomly everytime you place one
			  - ENTER places one tile at the cursor.
			  - SPACE places a rectangle of tiles; enter the
			    rectangle's corners just like the copy command.
			    Each individual tile in the rectangle will be
			    randomly selected from the active tile set.

	Tile editor (each level has its own set of tiles):
		Once more, some bottom row supplementing notes:
			- Color 14: the palette has one color that can be
			  different for each level. However, this color info
			  is not stored in the LEVEL?.SQZ file, so for now
			  the change won't appear in-game.
			- Tile property boxes: the 16*16 grid of tiles has
			  lines around them. The two white lines left and
			  right indicate a wall tile, white bottom indicates
			  a ceiling tile, white top indicates a floor tile.
			  When the tile is surrounded by a brownish box, it is
			  a bonus tile. When dots are around the tile,
			  it's animated.
			- Load/Save tiles: these load/save the entire set of
			  256 tiles. Load expects a 256*256*16 BMP file, Save
			  creates one. Also, the variable color (color 14)
			  will change to the most suitable color on a load.
			  The layout of the tiles in these BMP files is just
			  like how they are ordened in the editor, that is on
			  coordinates (0..15, 0..15) you'll find tile 0,
			  (240..255, 0..15) holds tile 15 and
			  (240..255, 240..255) contains tile 255.
			  This command only concerns the tile bitmaps, other
			  properties are not touched.
			- Import/Export tile: these import/export the
			  currently selected tile. Import expects a
			  16*16*16 BMP file, Export creates one. The variable
			  color is left untouched. Like Load/Save, only the
			  bitmaps are changed.
			- Set active tiles: the active tiles are the ones you
			  can place in the level editor with ENTER and SPACE.
			  If multiple active tiles are set, one will be
			  randomly chosen on placement.
			- Animated: an animated tile and the 2 tiles following
			  it are cycled. When you place one of the 2 tiles
			  following an animated tile, it'll also cycle
			  in-game. The tiles' properties don't cycle however,
			  only the bitmaps.
			- Bonus Type: Set the kind of bonus the tile holds.
			  Only effective when the tile is actually placed as a
			  bonus tile (B in the level editor). When it is
			  placed as a normal tile, the tiles have an
			  interesting zero gravity effect; also, the bonus
			  music plays on walk-by (except for Health). Apart
			  from these 2 things, nothing else happens.
			  One important thing: the last 3 tiles (253-255) are
			  always health bonuses, other tiles are never health
			  bonuses.
			- Wall/Floor/Ceiling properties: Dropdown is a tile
			  you fall through when crouching on top of it. Deadly
			  tiles have 3 varieties: one eats up all objects
			  dropped on top of it, one doesnt eat up at all while
			  the 3rd kind eats up most objects; the ball remains.
			  Ladders should have both the floor and the ceiling
			  set to ladder, although funny effects can be reached
			  by not doing this.

	Bonus/Nmi/Obj/Elevator/Gate editor:
	 General notes:
		- These work by selecting and manipulating items.
		  Initially, nothing will be selected, so follow
		  the instructions on-screen to select an item. Once
		  selected, using the arrows actually moves the item around.
		- Enter creates and selects a new item if the list is not full
		- Delete removes the selected item.
		- Clear all clears the list of items (but nothing else)
		- PgUp/PgDn moves through the list of items, while
		  Tab moves through the on-screen items.
		- Space deselects the item, so you can move around without
		  dragging the item along.
	 Bonus editor:
		- Use the "T" command to open the Tile editor and pick tiles
		  to place as bonus. Note that this doesn't affect the
		  currently selected bonus, only newly created ones.
	 Gate editor:
		- A gate has three coordinates: the entrance, the exit and the
		  position of the game screen when exiting the gate. Use E/X/P
		  to select one of the three.
		- After exiting a gate, scrolling can be enabled or disabled.
		  The scroll flag controls this.
	 Obj editor:
		- This uses sprites, which can be positioned pixel-precise.
		- Using "S" you can change the sprite of the object.
		- Using "F" you can flip the object horizontally.
	 Elevator editor:
		- Also uses sprites, but only 2 are available via "S"
		- You're placing the platform at one end of its traject; "D"
		  determines its direction, "T" its tempo and "R" its range.
	 NMI editor:
		- Sprite selecting works like the Obj editor
		- Use "B" to select a behavior for the enemy; each behavior
		  has its own variables to further configure. Use "N" to
		  quickly reset all of these values to normal, sensible values
		- Important note: when placing a sprite-generating enemy like
		  a shooter or dropper, make sure there is some place left in
		  the Obj editor's list. Otherwise, nothing will be
		  shot/dropped.
		- Some general parameters for most of the behaviors:
			- Power: determines how far the player will fly away
			  after being hit. Higher is farther. When the power
			  is 0, the player won't be hurt.
			- Tempo: speed of the enemy. Higher is faster.
		Following is a description of every behavior:
		1: Nothing
			Just stand, don't hurt the player
			- Immortal: can be killed or not
		2: Jump
			Jump up and down
			Example: fireballs in level 5
			- Jump power: how high to jump
			- Wait: delay between jumps
		3: Drop
			Drop objects on top of the player
			Example: stalactites in level 10
			- Wait: delay between drops
			- (Vertical) Zone: will only drop when the player is
			  within this zone
		4: Shoot
			Shoot horizontally at player
			Example: Hobo's in level 1
			- Wait: delay between shoots
			- Zone: will only shoot when the player is
			  within this zone
			- Direction: shoot only left, right, or both
		5: Walk and shoot
			Walk and shoot at player. Turn towards player if near.
			Example: catapult guys in level 5
			- Zone: will awake when player enters zone.
		6: Walk and periodically pop-up
			When popping up, turn towards player.
			Example: moles in level 8
			- Zone: will awake when player enters zone.
		7: Alert when near, walk when nearer
			Walking starts when the player is 50 pixels nearer.
			The small vertical lines indicate this distance.
			Example: dogs in level 1
			- Zone: will alert when player enters zone.
		8: Gravity walk, hit when near
			Falls down when no floor is below. When the player is
			 near a special animation plays (only visible with
			 arab enemies)
			Example: skeletons in level 5, arab in level 10
			- Zone: will awake when player enters zone.
		9: Gravity walk when off-screen
			Falls down when no floor is below. Awakes only when
			 player is not on-screen.
			Example: worms in level 4, boulders in level 7
			- Immortal: can be killed or not
		10: Noclip walk
			Just walk between two edges, ignoring any walls.
			Example: skinhead in level 1
			- Edges: select the edges between which to walk.
		11: Noclip walk, jump to player
			Like 10, but jump up to player when near.
			Example: fish at the end of level 12
			- Edges: select the edges between which to walk.
			- Vertical zone: won't jump higher than this.
		12: Noclip walk, move to player
			Like 10, but move vertically towards player when near.
			Example: bees in level 2
			- Edges: select the edges between which to walk.
			- Vertical zone: move when player is in this zone.
			Note: these don't move up/down when not on the screen,
			so don't make their vertical zones too big
		13: Bounce
			Jump towards player, won't reset its position when the
			 player is too far away.
			Example: level 11 boss
			- Wait: delay between jumps
			- Zone: jumps when player is in the zone
		14: Guard
			If player is in the zone, move towards him. Otherwise,
			move to initial position.
			Example: flies in level 10
			- Y-tempo: vertical speed of the enemy. Higher
			  is faster.
			- (Vertical) Zone: Moves to player when he is in the
			  zone.


Known versions:
 This is a list of known executable versions and some info about them.
 Moktar:
	Version A
		File date: 27 november 1991
		CRC-32: A83BE449
		Loader: Original (Best Protection Kit), does not seem to work
			in any windows version.
			This loader can cause trouble; MTF warns about this.
		Crack type: None
	Version B
		File date: 24 december 1996 or 23 june 2000? (needs to be confirmed)
		CRC-32: 4335E3C0
		Loader: None
		Crack type: Protection call converted to redundant instruction
	Version C
		File date: 04 july 2000? (needs to be confirmed)
		CRC-32: BAF077D7
		Loader: LZEXE
		Crack type: Protection routine returns immediately
	Version D
		File date: 21 december 1991
		CRC-32: D5A191A0
		Loader: None
		Crack type: Protection call replaced by NOP's
		Further info:
			This version has a built-in trainer, capable of:
			 - unlimited lives (press 1/2)
			 - god mode (press 3/4)
			 - stop enemies from hitting you (press 5/6)
			   (this'll also be in the upcoming improved trainer)
			 - level skip (press 7)
			Apart from that, it fixes the problem of messy AdLib
			music on fast pc's when using a physical sound card.
			This fix is also included in MTF now.
			Finally, the french texts in Moktar have been
			translated to english; most notably, the level names
			have changed. It appears that the translator missed
			level 12 though.
	Version E
		File date: 21 april 1992
		CRC-32: 8950A448
		Loader: Better original (ZIVpackED), works fine in windows.
		Crack type: Protection call replaced by NOP's
		Further info:
			The crack is contained within the loader; its code
			overwrites the string "ZIVpackED by Eric ZMIRO 1991".
			This suggests there's also a non-cracked ZIVpackED
			version out there.
	Version F
		File date: 27 november 1991
		CRC-32: 0634C105
		Loader: Original (Best Protection Kit), does not seem to work
			in any windows version.
			This loader can cause trouble; MTF warns about this.
		Crack type: None
		Further info:
			Same as version A, but has an extra 10 bytes at the
			end. It comes with a crack loader TSR (NEWCOME.COM)
			which has almost the same 10 bytes at the end, so it
			could be some kind of signature.
 Titus the Fox:
	Version A
		File date: 20 february 1992
		CRC-32: 3851F9C4
		Loader: Original (Best Protection Kit), does not seem to work
			in any windows version.
			This loader can cause trouble; MTF warns about this.
		Crack type: None
	Version B
		File date: 15 march 1992
		CRC-32: 5B24D2A4
		Loader: Better original (ZIVpackED), works fine in windows.
		Crack type: None
	Version C
		File date: 12 january 2005 or 10 february 2006? (needs to be confirmed)
		CRC-32: EEA26057
		Loader: None
		Crack type: Protection call replaced by NOP's
	Version D
		File date: 24 september 1996
		CRC-32: 31D8EFB4
		Loader: Better original (ZIVpackED), works fine in windows.
		Crack type: None
		Further info: Same as version B, but it's 11 bytes longer
	Version E
		File date: 15 march 1992
		CRC-32: C4F9B134
		Loader: Better original (ZIVpackED), works fine in windows.
		Crack type: None
		Further info: Same as version D but has an integrity check by
			"CARMEL Software Engineering - Turbo Anti-Virus(tm)"
	Version F
		File date: 18 august 1992
		CRC-32: 37D3B45B
		Loader: None
		Crack type: Protection call replaced by NOP's
		Further info: A trainer is built-in, which provides unlimited
			lives and can't be disabled. Additionally, the code
			generation routine is bypassed so that each level's
			code is equal to the internal level number.
	Version G
		File date: 1 january 1992
		CRC-32: 4F0BE8DF
		Loader: Intros by Yarden Games (text) & Cross Bow (graphics).
		Crack type: None
		Further info: Version B's code, with a double intro screen.
			The advertised crack tragically fails to work though,
			when run without MTF. On exit, it waits for a keypress.
	Version H
		File date: 30 march 1993
		CRC-32: 76C9D489
		Loader: Protection kit-B; this manages to crash DOSBox (0.72) itself.
			This loader can cause trouble; MTF warns about this.
		Crack type: None
		Further info: Untested as I couldn't get it to run without crashing!

Todo list (stuff that might be in a future version):
Let me know if you have any other suggestions!
	- Level viewer and sprite viewer can export to BMP
	In-game:
	 - Play Moktar with TTF's executable to solve Moktar's timing problems
	 - A maximum of 10 elevators on-screen, instead of 4
	 - Multiple bosses in a level working properly
	Editor:
	 - Loading or resizing a level preserves the Copy/Paste buffer
	 - Make scrolling a lot faster, by not redrawing the screen everytime
	 - Sprite editor. This will also replace the 'S' sprite select method;
	   selecting a sprite will be like the tile editor's ENTER command.
	 - NMI creator (define your own animations, since they depend on the
	   Behavior setting)
	 - Proper custom level file format, enabling creation of entire level
	   sets with modified title screen and all
	 - A second variable color. Noticed that colors 0 and 2 are the same?
	 - Edit things that are not stored in the LEVEL?.SQZ file, like the
	   variable colors, sprites, level name and music
	 - Level 14 bonus can hold code for other levels than 14
	 - Mouse support
	Trainer:
	 - Smooth movement (like in version 1.1)
	 - "Enabled" indicators
	 - Godmode: deadly tiles & bottom of screen no longer lethal
	 - Noclip: enemies don't cause you to fly off in all directions

Known bugs:
	- Gates in level 5: this is in fact a bug in the game itself! Try
	  entering the one that goes all the way down-right...
	- Option /F doesn't work with Moktar. Then again, the game itself
	  doesn't support this option, while TTF does.
	- In the game, when more than 4 elevators should be simultaneously
	  visible, only 4 can actually be seen. So, don't place more than
	  4 together for now.
	- When multiple bosses are placed in a level, the other boss(es) will
	  have 255 health after the first is killed.

Revision history:
 2.1.1	28-3-2013
	- New executables recognised: 3 TTF (F, G, H)
	- Bugfix: the level codes depend on CPU type (8086/286/386+), and the games
	  do run-time CPU detection. MTF's code viewer however always showed level
	  codes for a 386, causing wrong codes to be shown when running MTF on a 286 or
	  below. Now MTF detects the CPU type, so the correct codes are shown.
	- Fixed executable dates of Moktar (B, E) and TTF (C, E) in "Known versions"
	- Small improvement to MTF's patcher (it didn't work with TTF version G)
	- Version detection now uses CRC-32 and supports >64KB files. It also
	  displays the name of the found executable now.
	- CRC-32 is listed in the "Known versions" section; also added a warning text
	  there for problematic versions.
 2.1.0	11-1-2010
	- New executables recognised: 1 TTF (E), 1 Moktar (F)
	- Version A warning updated to include the new F version
	- New command line option PLAY to force running the game
	- Previously, most of the options prevented the menu with 4 choices
	  from appearing; from now on, the menu will always show unless
	  one of PLAY, TRN, COD, EDIT is entered
	- New command line option /R to change resolution in the editor
	- The three different deadly floor types actually do differ; the tile
	  editor shows their difference now in the Floor tile text
	- Fixed a bug where MTF COD sometimes showed nasty values for level number
	- Version detection uses CRC-16 instead of checksum, making it more robust
 2.0.2	29-1-2006
	- New executables recognised: 1 Moktar (E)
	- Fixed a bug in the executable recognising routine: when a file's
	  size matched but its checksum didn't, and no valid executable was
	  found, the 'No executable found' message became a mess of ASCII
	  characters.
	- Some of the NMI parameters got overwritten under certain
	  circumstances; this was most noticeable when placing "Guard" NMIs
	  which would not stay around their placed spot. It's fixed.
	- When placed as NMI, several sprites caused weird special effects
	  when playing. In Moktar, this affected the sprites: 197-202. In both
	  Moktar and TTF, the following sprites also behaved mischievously:
	   105 130 167 184 196 203 213 220 226 247 298-308.
	  All sprites work correctly now.
	- Fixed some silly 2005 numbers below here.
 2.0.1	8-1-2006
	- Fixed a copy/paste vs minimap bug in the editor; pasting more than
	  once would produce garbage, because the copy buffer was overwritten
	  with the updated minimap. Also, copying anything corrupted the
	  minimap temporarily.
	- Changed some default values after a "Clear" command to clear the
	  entire level; the player's initial position now no longer is
	  obscured by the minimap and the barrier is outside the level by
	  default.
	- The "Paste" command is now only shown when the paste buffer is
	  filled
	- Fixed a bug in the NMI, OBJ and Elevator editors where an object
	  that was near the top of the level would move to the level bottom
	  when selected.
	- Saving a level now confirms that writing succeeded.
 2.0	4-1-2006
	- Let's hope the windows 98 problem finally is solved now.
	  If it still explodes, remember that there is always DOSBox...
	- A level editor is included!
	- New executables recognised: 2 TTF (C, D), 2 Moktar (C, D)
	- Completely rewrote sprite drawing routine, which means clipping
	  works properly now and sprites can be drawn on any horizontal
	  position, not just on multiples of 8
	- Executable found message now includes version identifier;
	  this readme also contains some version information now
	- Changed type of crack, to avoid interfering with Moktar version D
	- Viewing the photo should not corrupt memory anymore
	- Now 100% 8086 assembly so the program works on every x86 CPU, like
	  the games do.
	- When Titus the Fox generates duplicate codes (like in DOSBox), an
	  alternate method of code generation is enabled
	- New command line option /A to force AdLib sound: useful in DOSBox
	- The AdLib music's strange behavior on fast pc's with non-emulated
	  sound cards should be fixed now (although this is untested: please
	  tell me if it makes any difference!)
	- Because on some keyboard layouts the level select keys ("<" and ">")
	  didn't work, they have been changed to PgUp and PgDn.
	- Added warning when MTF finds version A. It can be disabled with the
	  new command-line option /W
	- Corrected this revision history's information about 1.0 and 1.1
	Level viewer:
	 - New command 'S', toggling display of tile solidity
	 - Removed display of player-sprite in inappropriate places, when
	   Elevators are visible
	 - Play command: when exiting the game you'll return to the viewer;
	   also, when you finish the level in the game or get a "game over"
	   the game will automatically exit
	 - Wrongly placed objects on the minimap (like level 5's gate lines)
	   are now clipped
	 - Fixed a line clipping bug with vertical lines
 1.99	5-1-2005
	- Changed name to Moktar the Fox
	- Now also works with Moktar
	- Automatically searches for and identifies main game executable
	  Versions known: 2 Moktar, 2 TTF
	- Added level viewer and other viewers
	- If no parameters are given, displays a menu
	- Updated code viewer to work with different versions
	Trainer:
	 - Changed internal workings a lot
	 - Noclip mode works a bit jerky (will be fixed)
	 - Health & lives decrease all the way down
	 - Future version will contain an improved trainer
 1.1	28-12-2003
	- Added code viewer
	Trainer:
	 - Can now be toggled in-game via F5, F6, F7
	 - Added noclip mode
 1.0	17-7-2002
	Initial release
	Features:
	 - Trainer: godmode, infinite lives

Disclaimer:
 I can not be held responsible if your computer decides to explode or displays
 similar unorthodox behaviour.

Look at http://ttf.mine.nu for more Titus the Fox / Moktar related stuff.

Questions? Problems? Requests? Spam? Send to mail@ttf.mine.nu
