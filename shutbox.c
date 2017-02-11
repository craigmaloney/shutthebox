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

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dice.h"		/* Dice images */
#include "tile.h"		/* Tile images */
#include "shutbox.h"		/* Shut the Box */

gint CloseAppWindow (GtkWidget * widget, gpointer * data) {
	gtk_main_quit ();
	return (FALSE);
}

/* 
 * CreateWidgetFromXpm
 *
 * Convert an xpm string into a pixmap widget
 * Uses (win_main) which is a global.
 *
 * From the "Developing Linux Applications" book by Eric Harlow
 */

GtkWidget * CreateWidgetFromXpm (GtkWidget * window, gchar ** xpm_data) {
	GdkBitmap *mask;
	GdkPixmap *pixmap_data;
	GtkWidget *pixmap_widget;

	pixmap_data = gdk_pixmap_create_from_xpm_d (
			window->window,
			&mask,
			NULL,
			(gchar **) xpm_data);

	pixmap_widget = gtk_pixmap_new (pixmap_data, mask);
	gtk_widget_show (pixmap_widget);

	return (pixmap_widget);
}

void SetDiceTotalLabel (int dicetotal) {
	char dice_label_char[20];

	sprintf (dice_label_char, "Dice Total: %d", dicetotal);
	gtk_label_set (GTK_LABEL (dice_label), dice_label_char);
	gtk_widget_show (dice_label);
}

void SetTileTotalLabel (int tiletotal) {
	char tile_label_char[20];

	sprintf (tile_label_char, "Tile Total: %02d  ", tiletotal);
	gtk_label_set (GTK_LABEL (tile_label), tile_label_char);
	gtk_widget_show (tile_label);
}

void SetRoundLabel (int round_value, GtkWidget * round_label) {
	char label[20];

	sprintf (label, "Round: %d", round_value);
	gtk_label_set (GTK_LABEL (round_label), label);
}

void SetScoreLabel (int score_value, GtkWidget * score_label) {
	char label[20];

	sprintf (label, "Score: %d", score_value);
	gtk_label_set (GTK_LABEL (score_label), label);
}

void FreeChildCallback (GtkWidget * widget) {
	gtk_widget_destroy (widget);
}

void FreeChild (GtkWidget * widget) {
	gtk_container_foreach (
			GTK_CONTAINER (widget),
			(GtkCallback) FreeChildCallback,
			NULL);
}


void TileButtonClicked (GtkWidget * widget, gpointer * data) {
	typeTileButton *boxButton;
	int sum=0;
	int i;

	boxButton = (typeTileButton *) data;
	boxButton->toggle = !boxButton->toggle;

	for (i = 1; i <= 9; i++) {

		if (TileButtons[i].active) {
			sum += (TileButtons[i].toggle * i);
		}
	}
	SetTileTotalLabel(sum);
	
# ifdef DEBUGGING
	printf ("Value %d, Toggle %d\n", boxButton->value, boxButton->toggle);
# endif

}

void SetTileButtonsInactive (typeTileButton * boxButton, int i) {
	typeTileButton *tempButton;

	tempButton = &boxButton[i];
	if ((tempButton->active) && (tempButton->toggle)) {
		tempButton->active = 0;
		tempButton->toggle = 0;
		gtk_widget_set_sensitive (tempButton->widget, 0);
		gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (tempButton->widget), 0);
	}
}

int TallyScore (typeTileButton * boxButton) {
	int i;
	int sum = 0;
	typeTileButton *tempButton;

	for (i = 1; i <= 9; i++) {
		tempButton = &boxButton[i];
		sum = sum + ((tempButton->toggle && tempButton->active) * tempButton->value);
	}
	return sum;
}

int CheckEndGame (int die_sum) {
	int i;
	int orig_sum = die_sum;
	int temp_value;
	int result;
	if (die_sum == 0) {
		return 1;
	}

	if (die_sum >= 9) {
		die_sum = 9;
	}

	for (i = die_sum; i >= 1; i--) {
		if (TileButtons[i].active) {
			temp_value = (orig_sum - i);
			if (temp_value >= i) {
				return 0;
			}

			result = CheckEndGame(temp_value);
			if (result) {
				return 1;
			}
		}
	}
	return 0;
}
		
void RollDie () {
	GtkWidget *tempWidget;

	FreeChild (die1.widget);
	FreeChild (die2.widget);
	die1.value = (1 + (rand () % 6));
	tempWidget = CreateWidgetFromXpm(window, (gchar **) dice_image[die1.value]);
	gtk_container_add(GTK_CONTAINER(die1.widget), tempWidget);
	die2.value = (1 + (rand() % 6));
	tempWidget = CreateWidgetFromXpm (window, (gchar **) dice_image[die2.value]);
	gtk_container_add (GTK_CONTAINER(die2.widget), tempWidget);
	gtk_widget_show(die1.widget);
	gtk_widget_show(die2.widget);
	SetDiceTotalLabel(die1.value + die2.value);
}

void ClearTileButtons (typeTileButton * boxButton) {
	int i;

	for (i = 1; i <= 9; i++) {
		if (boxButton[i].active && boxButton[i].toggle) {;
			gtk_widget_set_sensitive (boxButton[i].widget, 1);
			gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (boxButton[i].widget), 0);
		}
	}
}


void Clear (GtkWidget * widget, gpointer * data) {
	ClearTileButtons (TileButtons);
}


void ResetTileButtons (typeTileButton * boxButton) {
	int i;

	for (i = 1; i <= 9; i++) {
		boxButton[i].active = 1;
		boxButton[i].toggle = 0;
		gtk_widget_set_sensitive (boxButton[i].widget, 1);
		gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (boxButton[i].widget), 0);
	}
}

void ResetGame (void) {
	ClearTileButtons (TileButtons);
	ResetTileButtons (TileButtons);
	round_number = 1;
	score = 0;
	SetRoundLabel (round_number, round_label);
	SetScoreLabel (score, score_label);
	RollDie ();
}

void ResetGameEvent (GtkWidget * widget, gpointer * data) {
	ResetGame();
	gtk_widget_hide_all(dialog_won);
	gtk_widget_hide_all(dialog_lost);
}

void RollEvent (GtkWidget * widget, gpointer * data) {
	int k;
	int toggle_sum = 0;
	int ButtonValue;

	ButtonValue = TallyScore (TileButtons);
	if (ButtonValue == (die1.value + die2.value)) {
		score += ButtonValue*round_number;
		for (k = 1; k <= 9; k++) {
			SetTileButtonsInactive (TileButtons, k);
		}
		RollDie ();
		for (k = 1; k <= 9; k++) { 
			toggle_sum += TileButtons[k].active;
		}

		if (toggle_sum == 0) {
			gtk_widget_show_all(dialog_won);
			ResetGame();
		}
		round_number++;
		SetRoundLabel (round_number, round_label);
		SetScoreLabel (score, score_label);
	}
	if (CheckEndGame(die1.value+die2.value) == 0) {
		gtk_widget_show_all(dialog_lost);
		ResetGame();
	}
}

void PaintTileButtons (GtkWidget * tempButton, gint value) {
	GtkWidget *tempWidget;
	tempWidget = CreateWidgetFromXpm (window, (gchar **) tile_image[value]);
	gtk_container_add (GTK_CONTAINER (tempButton), tempWidget);
}


int main (int argc, char *argv[]) {
	gint i;

	gtk_init (&argc, &argv);

	/* Randomize things up a bit */
	srand (time (NULL));

	/* Give the Dice some initial values */
	die1.value = 1;
	die2.value = 1;
	/* Initialize the score and round_number */
	round_number = 1;
	score = 0;

	/* Create the Winner Dialog Window */

	dialog_won = gtk_dialog_new();
	gtk_widget_set_usize (dialog_won, 214, 117 );
	gtk_window_set_policy (GTK_WINDOW(dialog_won), FALSE, FALSE, FALSE);
	gtk_window_set_title (GTK_WINDOW(dialog_won), "You Win!");
	dialog_label = gtk_label_new("You've Shut the Box!");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog_won)->vbox), dialog_label, TRUE, TRUE, 0);
	button = gtk_button_new_with_label ("Woo Hoo!");
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG (dialog_won)->action_area), button, FALSE, FALSE, 0);
	g_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (ResetGameEvent), "Reset");gtk_widget_grab_default (button);

	dialog_lost = gtk_dialog_new();
	gtk_widget_set_usize (dialog_lost, 214, 117 );
	gtk_window_set_policy (GTK_WINDOW(dialog_lost), FALSE, FALSE, FALSE);
	gtk_window_set_title (GTK_WINDOW(dialog_lost), "You lost.");
	dialog_label = gtk_label_new("Game over.");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog_lost)->vbox), dialog_label, TRUE, TRUE, 0);
	button = gtk_button_new_with_label ("Bummer");
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG (dialog_lost)->action_area), button, FALSE, FALSE, 0);
	g_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (ResetGameEvent), "Reset");gtk_widget_grab_default (button);

	/* Create the window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), title_label);
	g_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);
	gtk_container_border_width (GTK_CONTAINER (window), 50);

	/* Set up the vertical box */
	vbox = gtk_vbox_new (FALSE, 0);

	/* Set up the tiles */
	button_hbox = gtk_hbox_new (FALSE, 0);
	for (i = 1; i <= 9; i++) {
		TileButtons[i].value = i;
		TileButtons[i].toggle = 0;
		TileButtons[i].active = 1;
		TileButtons[i].widget = gtk_toggle_button_new ();
		gtk_widget_set_usize (TileButtons[i].widget, 32, 32);
		gtk_box_pack_start (GTK_BOX (button_hbox), TileButtons[i].widget, FALSE, FALSE, 0);
		gtk_widget_show (TileButtons[i].widget);
		g_signal_connect (GTK_OBJECT (TileButtons[i].widget), "clicked",
				GTK_SIGNAL_FUNC (TileButtonClicked), &TileButtons[i]);
	}
	gtk_container_add (GTK_CONTAINER (vbox), button_hbox);
	gtk_widget_show (button_hbox);

	/* Handle the dice */
	dice_hbox = gtk_hbox_new (TRUE, 0);
	die1.widget = gtk_button_new ();
	die2.widget = gtk_button_new ();
	gtk_box_pack_start (GTK_BOX (dice_hbox), die1.widget, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (dice_hbox), die2.widget, FALSE, FALSE, 0);
	gtk_widget_show (die1.widget);
	gtk_widget_show (die2.widget);
	tile_label = gtk_label_new ("Tile Total: 00  ");
	gtk_box_pack_start (GTK_BOX (dice_hbox), tile_label, FALSE, FALSE, 0);
	gtk_widget_show (tile_label);
	dice_label = gtk_label_new ("Dice Total: 0");
	gtk_box_pack_start (GTK_BOX (dice_hbox), dice_label, FALSE, FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox), dice_hbox);
	gtk_widget_show (dice_label);
	gtk_widget_show (dice_hbox);

	/* Label row */
	label_hbox = gtk_hbox_new (TRUE, 0);
	score_label = gtk_label_new ("Score: 0");
	gtk_box_pack_start (GTK_BOX (label_hbox), score_label, FALSE, FALSE, 0);
	gtk_widget_show (score_label);
	round_label = gtk_label_new ("Round: 1");
	gtk_box_pack_start (GTK_BOX (label_hbox), round_label, FALSE, FALSE, 0);
	gtk_widget_show (round_label);
	gtk_container_add (GTK_CONTAINER (vbox), label_hbox);
	gtk_widget_show (label_hbox);

	/* Action buttons */
	action_hbox = gtk_hbox_new (TRUE, 0);
	button = gtk_button_new_with_label ("Roll");
	g_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (RollEvent), "Roll Clicked");
	gtk_box_pack_start (GTK_BOX (action_hbox), button, FALSE, FALSE, 0);
	gtk_widget_show (button);
	button = gtk_button_new_with_label ("Clear Tiles");
	g_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (Clear), "Clear");
	gtk_box_pack_start (GTK_BOX (action_hbox), button, FALSE, FALSE, 0);
	gtk_widget_show (button);
	button = gtk_button_new_with_label ("New Game");
	g_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (ResetGameEvent), "Reset");
	gtk_box_pack_start (GTK_BOX (action_hbox), button, FALSE, FALSE, 0);
	gtk_widget_show (button);
	gtk_container_add (GTK_CONTAINER (vbox), action_hbox);
	gtk_widget_show (action_hbox);

	/* Show the vertical box */
	gtk_container_add (GTK_CONTAINER (window), vbox);
	gtk_widget_show (vbox);

	gtk_widget_show (window);

	for (i = 1; i <= 9; i++) {
		PaintTileButtons (TileButtons[i].widget, i);
	}
	RollDie ();

	gtk_main ();
	return (0);
}
