/* Shut the Box
 * Written by Craig Maloney
 * A dice solitaire game.
 *
 * Copyright (C) 2002, 2004 Craig Maloney 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

char title_label[20] = "Shut the Box 0.4"; 
typedef struct {
	GtkWidget *widget;		/* The button itself */
	int value;
	int toggle;			/*Toggled flag */
	int active;			/*'has this widget been shut?' flag */
}
typeTileButton;

typedef struct {
	GtkWidget *widget;		/* The individual die */
	int value;
}
dice;

GtkWidget *window;
GtkWidget *button;
GtkWidget *button_hbox;		/* button widget container */
GtkWidget *label_hbox;		/* label widget container */
GtkWidget *dice_hbox;		/* dice and dice total label container */
GtkWidget *action_hbox;		/* Roll / Reset Button container */
GtkWidget *vbox;
GtkWidget *dice_label;
GtkWidget *tile_label;
GtkWidget *score_label;
GtkWidget *dialog_label;
GtkWidget *dialog_won;
GtkWidget *dialog_lost;
GtkWidget *round_label;
GtkWidget *die_pixmap1, *die_pixmap2;
dice die1;
dice die2;
int round_number;
int score;
typeTileButton *tempButton;

typeTileButton TileButtons[13];

/* Pointer array to the images found in dice.h */
gpointer dice_image[] = {
	NULL,
	xpm_die1,
	xpm_die2,
	xpm_die3,
	xpm_die4,
	xpm_die5,
	xpm_die6
};

/* Pointer array to the images found in tile.h */
gpointer tile_image[] = {
	NULL,
	tile_01_xpm,
	tile_02_xpm,
	tile_03_xpm,
	tile_04_xpm,
	tile_05_xpm,
	tile_06_xpm,
	tile_07_xpm,
	tile_08_xpm,
	tile_09_xpm,
};

gint CloseAppWindow (GtkWidget * widget, gpointer * data);
GtkWidget * CreateWidgetFromXpm (GtkWidget * window, gchar ** xpm_data);
void SetDiceTotalLabel (int dicetotal);
void SetTileTotalLabel (int tiletotal);
void SetRoundLabel (int round_value, GtkWidget * round_label);
void SetScoreLabel (int score_value, GtkWidget * score_label);
void FreeChildCallback (GtkWidget * widget);
void FreeChild (GtkWidget * widget);
void TileButtonClicked (GtkWidget * widget, gpointer * data);
void SetTileButtonsInactive (typeTileButton * boxButton, int i);
int TallyScore (typeTileButton * boxButton);
int CheckEndGame (int die_sum);
void RollDie (void);
void ClearTileButtons (typeTileButton * boxButton);
void Clear (GtkWidget * widget, gpointer * data);
void ResetTileButtons (typeTileButton * boxButton);
void ResetGame (void);
void ResetGameEvent (GtkWidget * widget, gpointer * data);
void RollEvent (GtkWidget * widget, gpointer * data);
void PaintTileButtons (GtkWidget * tempButton, gint value);
