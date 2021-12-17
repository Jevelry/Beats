// Assignment 2 20T1 COMP1511: CS bEats
// beats.c
//
// This program was written by Kevin Huang (z5311917)
// on ? (forgot lol)
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Fix default return value of add_musical_note_to_beat.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Add any extra #includes your code needs here.

#include "beats.h"

// Add your own #defines here.

////////////////////////////////////////////////////////////////////////

struct track {
    struct beat *head;
    struct beat *current_selected;
};

struct beat {
    struct note *notes;
    struct beat *next;
};

struct note {
    int octave;
    int key;
    struct note *next;
};

////////////////////////////////////////////////////////////////////////

// Return a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);
    new_beat->next = NULL;
    new_beat->notes = NULL;
    return new_beat;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////


// SORRY FOR THE SLOPPY COMMENTING :(
//     ___
// __/_  `.  .-"""-.
// \_,` | \-'  /   )`-')
//  "") `"`    \  ((`"` //**
// ___Y  ,    .'7 /|    //***
//(_,___/...-` (_/_/

// Add a note to the end of a beat.
int add_note_to_beat(Beat beat, int octave, int key) { 
            
    struct note *new_note = malloc(sizeof(struct note));
    new_note->next = NULL;
    // Return INVALID OCTAVE if input is out of bounds
    if (octave >= 10 || octave < 0) {      
        free (new_note);
        return INVALID_OCTAVE;        
    }
    // Return INVALID KEY if input is out of bounds
    else if (key >= 12 || key < 0) {       
        free (new_note);
        return INVALID_KEY;
    }
    else {
        // If linked list is empty, create a new note as the head of list
        if (beat->notes == NULL) {
            beat->notes = new_note;
            new_note->octave = octave;
            new_note->key = key;  
            return VALID_NOTE;
        }
        // Check if new Note is higher than the Notes in the beat
        struct note *current = beat->notes;
        struct note *prev = current;              
        while (current != NULL) {
            if (current->octave < octave) {
            } 
            else if (current->octave == octave) {
                if (current->key >= key) {
                    free (new_note);
                    return NOT_HIGHEST_NOTE;
                }                
            }else {
                free (new_note);
                return NOT_HIGHEST_NOTE;
            }
            prev = current;
            current = current->next;
        }
        // Insert new_note
        prev->next = new_note;                  
        new_note->octave = octave;
        new_note->key = key; 
    }              
    return VALID_NOTE;
}

// Print the contents of a beat.
void print_beat(Beat beat) {    
    if (beat->notes == NULL) {
    printf("\n");
    }
    else {
        struct note *current = beat->notes;
        while (current != NULL) {
            printf("%d ", current->octave);
            if (current->key < 10) {
                printf("0%d ", current->key);
            }else {
                printf("%d ", current->key);
            }
            if (current->next != NULL) {
            printf("| ");
            }
            current = current->next;   
        }    
    printf("\n");
    }
}

// Count the number of notes in a beat that are in a given octave.
int count_notes_in_octave(Beat beat, int octave) {
    int count = 0;
    // Reject invalid octave
    if (octave > 10 || octave < 0) {              
        return 0;
    }       
    struct note *current = beat->notes;
    while (current != NULL) {
        if (current->octave == octave) {
        count++;
        }
        current = current->next;
    }
    return count;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

// Return a malloced track with fields initialized.
Track create_track(void) {
    struct track *new_track = malloc(sizeof(struct track));
    new_track->head = NULL;
    new_track->current_selected = NULL;
    return new_track;
}

// Add a beat after the current beat in a track.
void add_beat_to_track(Track track, Beat beat) {           
    // If a Track is 'playing' (it has a currently selected Beat),
    // insert the given Beat into the Track after the currently selected Beat.
    if (track->current_selected != NULL) {
        beat->next = track->current_selected->next;
        track->current_selected->next = beat;
    }
    // If a Track is 'stopped' (it has no currently selected Beat),
    // add the Beat as the first element of the Track.
    if (track->current_selected == NULL) {
        beat->next = track->head;
        track->head = beat;
    }                             
}

// Set a track's current beat to the next beat.
int select_next_beat(Track track) {    
    // If the Track is 'stopped', and contains no beats
    if (track->current_selected == NULL && track->head == NULL) { 
        return TRACK_STOPPED;
    }
    
    // If the Track is 'stopped', and contains at least one Beat,
    // the first Beat in the track should become the selected Beat.
    if (track->current_selected == NULL && track->head != NULL) { 
        track->current_selected = track->head;
        return TRACK_PLAYING;
    }
             
    // If the currently selected beat in the track is the last Beat
    // currently in the track, the track should become 'stopped', and
    // it should have no currently selected Beat.
    if (track->current_selected->next == NULL) {
        track->current_selected = NULL;
        return TRACK_STOPPED;
    }    
    track->current_selected = track->current_selected->next;
    return TRACK_PLAYING;       
}

// Print the contents of a track.
void print_track(Track track) {
    // If the Track currently has no Beat, do not print anything.
    if (track->head == NULL) {
        return;
    }
    // Print one line for each Beat in the Track, containing:
    // '>' as the first character, if the Beat is the currently selected Beat.
    // otherwise, a ' '.
    struct beat *current = track->head;
    int index = 1;
    while (current != NULL) {
        if (current == track->current_selected) {
            printf(">");
        } else {
            printf(" ");
        }        
        // '[', then the index of this Beat, starting at 1, then by a ']', and a ' '.    
        printf("[%d] ", index);
        // If the Beat has any Notes, each Note in the Beat, in ascending order
        print_beat(current);        
        index++;
        current = current->next;
    }            
}

// Count beats after the current beat in a track.
int count_beats_left_in_track(Track track) {
    int beat_count = 0;
    
    // If track is stopped, return 0
    if (track->current_selected == NULL) {
        if (track->head == NULL) {
            return 0;
        }
        // Return number of beats in the track.
        struct beat *current = track->head;
        while (current != NULL) {
            beat_count++;
            current = current->next;
        }
    }
    // Return the number of beats after the currently selected beat
    else {
        struct beat *current = track->current_selected;
        while (current != NULL) {
            beat_count++;
            current = current->next;
        }
        // Compensate for including selected beat
        beat_count--;            
    }         
    return beat_count;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

// Free the memory of a beat, and any memory it points to.
void free_beat(Beat beat) {
    // If linked list is empty, exit
    if (beat == NULL) {
        return;
    }
    // Loop through and free all notes
    struct note *current = beat->notes;
    struct note *prev = NULL;
    while (current != NULL) {
        free(prev);
        prev = current;
        current = current->next;
    }
    free (prev);
    free (beat);
    return;
}

// Free the memory of a track, and any memory it points to.
void free_track(Track track) {
    // If linked list is empty, exit
    if (track == NULL) {
    return;
    }
    // Loop through and free all beats and notes
    struct beat *current = track->head;
    struct beat *prev = NULL;
    while (current != NULL) {
        free_beat(prev);
        prev = current;
        current = current->next;
    }
    free_beat (prev);
    free (track);
    return;
}

// Remove the currently selected beat from a track.
int remove_selected_beat(Track track) {
    // If the Track is stopped, this function should do nothing.
    if (track->current_selected == NULL) {
    return TRACK_STOPPED;
    }  
    
    // In general, if there is no next Beat
    if (track->current_selected->next == NULL) {
        struct note *current = track->current_selected->notes;
        struct note *prev = NULL;
        while (current != NULL) {            
            prev = current;
            current = current->next;
            free(prev);
        }
        // If beat is only note in track, remove and stop track
        if (track->current_selected == track->head) {
            free (track->current_selected);
            track->head = NULL;
            track->current_selected = NULL;
            return TRACK_STOPPED;
        }
        // If selected note in last, Find second last beat
        struct beat *before_selected = track->head;
        while (before_selected->next != track->current_selected) {
            before_selected = before_selected->next;
        }
        // Remove last note and make second last note point to NULL
        before_selected->next = NULL;
        free (track->current_selected);
        track->current_selected = NULL;
        return TRACK_STOPPED;
    }
    // If beat is first in track and there are other beats after
    if (track->current_selected == track->head) {
        struct note *current = track->current_selected->notes;
        struct note *prev = NULL;
        while (current != NULL) {            
            prev = current;
            current = current->next;
            free(prev);
        }
        track->head = track->current_selected->next;
        free (track->current_selected);
        track->current_selected = track->head;
        return TRACK_PLAYING;
    }
    
    // If the Track is playing, this function should remove the
    // currently selected Beat from the Track.
    else {
        struct note *current = track->current_selected->notes;
        struct note *prev = NULL;
        while (current != NULL) {
            prev = current;
            current = current->next;
            free(prev);
        }      
        // The next Beat should become the selected Beat.
        struct beat *before_selected = track->head;
        while (before_selected->next != track->current_selected) {
            before_selected = before_selected->next;
        }
        before_selected->next = track->current_selected->next;
        free (track->current_selected);
        track->current_selected = before_selected->next;
        return TRACK_PLAYING;
    }
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

// Add note to beat, given its 'musical notation'.
int add_musical_note_to_beat(Beat beat, char *musical_notation) {
    struct note *new_note = malloc(sizeof(struct note));
    new_note->next = NULL;
    int octave = 0;
    int key = 0;
    int sharp_key = 0;    
    
    //  - The first character is a digit from '0' to '9'.
    octave = strtol(&musical_notation[0], NULL, 10);
    if (octave < 0 || octave > 9) {             
        free (new_note);
        return INVALID_MUSICAL_NOTE;        
    }
    //  - The second character is a capital letter from 'A' to 'G'
    int letter = musical_notation[1];
    //The second character (the letter) should be translated to a number,

    if (letter == 'A') {
        key = 0;
    } else if (letter == 'B') {
        key = 2;
    } else if (letter == 'C') {
        key = 3;
    } else if (letter == 'D') {
        key = 5;
    } else if (letter == 'E') {
        key = 7;
    } else if (letter == 'F') {
        key = 8;
    } else if (letter == 'G') {
        key = 10;
    } else {
        free (new_note);
        return INVALID_MUSICAL_NOTE;
    }   
     
    // Any remaining characters must be the character '#'.
    // For every '#', add one to the Note. If the Note overflows to be 
    // bigger than 11, subtract 12, and add one to the Octave.
    int counter = 2;
    while (counter < strlen(musical_notation)) {
        if (musical_notation[counter] == '#') {            
            sharp_key++;
            counter++;
        } else {
            free (new_note);
            return INVALID_MUSICAL_NOTE;
        }
    }
    while (sharp_key > 0 || key > 11) {
        if (sharp_key > 11) {
        sharp_key = sharp_key - 12;      
        octave++;
        }
        if (sharp_key <= 11) {
            key = key + sharp_key;
            sharp_key = 0;
        }
        if (key > 11) {
            key = key - 12;
            octave++;
        }
    }
    // If list is NULL, make new beat head   
    if (beat->notes == NULL) {
        beat->notes = new_note;
        new_note->octave = octave;
        new_note->key = key;  
        return VALID_NOTE;
    }
    // Find point to insert new note
    struct note *current = beat->notes;
    struct note *prev = NULL;
    int over = 0;              

    // should add a Note even if it is not the highest Note in the Beat.
    // Loop through and find point to insert note so notes are ascending
    while (current != NULL && over == 0) {
        if (current->octave < octave) {
            prev = current;
            current = current->next; 
        }
        // Should not add same note 
        else if (current->octave == octave) {
            if (current->key == key) {
                free (new_note);
                return INVALID_MUSICAL_NOTE;
            }
            if (current->key > key) {
                over = 1;
            }
            else {
                prev = current;
                current = current->next;
            }
        }  
        else {
            over = 1;
        }          
    }    
    // If new note is first in list
    if (prev == NULL) {
        new_note->next = current;
        beat->notes = new_note;
        new_note->octave = octave;
        new_note->key = key; 
        return VALID_NOTE;
    } 
    // Else
    else {                     
        prev->next = new_note;                  
        new_note->octave = octave;
        new_note->key = key; 
        if (current != NULL) {
            new_note->next = current;
        } 
    }             

    return VALID_NOTE;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

// Cut a range of beats to the end of a track.
void cut_range_to_end(Track track, int range_length) {
    
    struct beat *current = track->head;
    struct beat *prev = NULL;
    struct beat *before_selected = NULL;
    struct beat *selected_beat = NULL;
    int found = 0;
    int range_counter = range_length;
    
    if (track->current_selected == NULL || track->current_selected->next == NULL 
        || range_length == 0) {
        return;
    }
    // Loop through to find selected beat_count
    // Store selected beat and previous beat
    while (current != NULL && found == 0) {
        if (current == track->current_selected) {
            if (prev == NULL) {
                before_selected = NULL;
                selected_beat = current;
                found = 1;
            } else {
                before_selected = prev;
                selected_beat = current;
                found = 1;            
            }
        } else {                 
            prev = current;
            current = current->next;
        }
    } 
    // Loop through to end of range  
    while (current != NULL && range_counter > 0) {        
        prev = current;
        current = current->next; 
        range_counter--;  
    }
    // Exit as cutting length > beats does nothing
    if (current == NULL) {
        return;
    }
    // If selected beat is first, make beat after end of range the head
    if (before_selected == NULL) {
        track->head = current;
    } else { // Connect two ends, skipping the cut out beats
        before_selected->next = current;
    } // Go to end
    while (current != NULL) { 
        prev = current;   
        current = current->next;
    } // Connect end with range that was cut out   
    prev->next = selected_beat;
    current = selected_beat;
    // Find last note and make it point to NULL     
    while (range_counter < range_length) {
        range_counter++;
        prev = current;
        current = current->next;
    }     
    prev->next = NULL;
    return;
}

// Reverse a list of beats within a range of a track.
int reverse_range(Track track, int range_length) {
        
    struct beat *current = track->head;
    struct beat *prev = NULL;
    struct beat *before_select_beat = NULL;
    struct beat *select_beat = NULL;
    int found = 0;
    int range_counter = range_length;
            
    if (track->current_selected == NULL || track->current_selected->next == NULL 
        || range_length <= 0) {
        return 0;
    }
    // Loop through to find selected beat_count
    // Store selected beat and previous beat
    while (current != NULL && found == 0) {
        if (current == track->current_selected) {
            if (prev == NULL) {
                before_select_beat = NULL;
                select_beat = current;
                found = 1;
            } else {
                before_select_beat = prev;
                select_beat = current;
                found = 1;            
            }
        } else {                 
            prev = current;
            current = current->next;
        }
    }
    
    int beats_count = count_beats_left_in_track(track);
    struct beat *previous = NULL;
    struct beat *curr = select_beat;
    struct beat *next = NULL;
    // Reverses selected range of beats
    while (curr != NULL && range_counter > 0) {
        next = curr->next;
        curr->next = previous;
        previous = curr;
        curr = next;
        range_counter--;
    }
    // If selected beat was first, make new head start of reversed beats range
    if (before_select_beat == NULL) {
        track->head = previous;
    } else { // Else make beat before currently 
             // selected point to start of reversed beats range
        before_select_beat->next = previous;
    } 
    // Connect end of reversed beats to rest of track
    select_beat->next = next; 
    if (beats_count < range_length) {
        return beats_count;
    }               
    return range_length - 1;    
}
