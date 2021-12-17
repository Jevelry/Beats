// Assignment 2 20T1 COMP1511: CS bEats
// test_beats.c
//
// This program was written by YOUR-NAME-HERE (z5555555)
// on INSERT-DATE-HERE
//
// Version 1.0.0: Assignment released.
#include <stdio.h>
#include <stdlib.h>

#include "test_beats.h"
#include "beats.h"


// Test function for `add_note_to_beat`
int test_add_note_to_beat(void) {
    // Test 1: Rejecting negative inputs.
    Beat test_beat = create_beat();
    if (add_note_to_beat(test_beat, -1, -1) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, -1, 0) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 1, -1) != INVALID_KEY) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2: Rejecting smaller input
    add_note_to_beat(test_beat, 2, 2); 
    if (add_note_to_beat(test_beat, 1, 1) != NOT_HIGHEST_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 3: Rejecting same input
    if (add_note_to_beat(test_beat, 2, 2) != NOT_HIGHEST_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
   
    // Test 3: Check if notes are ascending
    add_note_to_beat(test_beat, 2, 5); 
    add_note_to_beat(test_beat, 3, 5); 
    add_note_to_beat(test_beat, 4, 5); 
    add_note_to_beat(test_beat, 5, 5); 
    if (add_note_to_beat(test_beat, 5, 4) != NOT_HIGHEST_NOTE) {
        return DOES_NOT_MEET_SPEC;   
    }
    
    return MEETS_SPEC;
    
}

// Test function for `count_notes_in_octave`
int test_count_notes_in_octave(void){
    Beat test_beat = create_beat();
    // Test 1: Given beat with no Notes
    if (count_notes_in_octave(test_beat, 1) != 0) {
        return DOES_NOT_MEET_SPEC;
    }             
    // Test 2: Counting invalid octaves.
    add_note_to_beat(test_beat, 1, 1);
    if (count_notes_in_octave(test_beat, -1) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(test_beat, 11) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    // Test 3: Counting correct number of notes
    add_note_to_beat(test_beat, 1, 2);
    add_note_to_beat(test_beat, 1, 2);
    add_note_to_beat(test_beat, 1, 3);
    if (count_notes_in_octave(test_beat, 1) == 4 && count_notes_in_octave(test_beat, 2) == 0) {
        return MEETS_SPEC;  
    }

    return MEETS_SPEC;
}

// Test function for `add_beat_to_track`
int test_add_beat_to_track(void){
    Track test_track = create_track();
    Beat test_beat = create_beat();
    // Test 1: If a Track is 'stopped' Beat is first element    
    add_beat_to_track(test_track, test_beat);
    if (count_beats_left_in_track (test_track) != 1) {
        return DOES_NOT_MEET_SPEC;
    }
   
    // Test 2: Adding to a current beat
    select_next_beat(test_track);
    Beat test_beat2 = create_beat();
    add_beat_to_track(test_track, test_beat2);
    if (count_beats_left_in_track (test_track) != 1) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: Adding to last beat_count
    select_next_beat(test_track);
    Beat test_beat3 = create_beat();
    add_beat_to_track(test_track, test_beat3);
    if (count_beats_left_in_track (test_track) != 1) {
        return DOES_NOT_MEET_SPEC;
    }


    return MEETS_SPEC;
}

// Test function for `remove_selected_beat`
int test_remove_selected_beat(void){
    Track test_track = create_track();
    Beat test_beat = create_beat();

    // Test 1: Check if works with first note    
    add_beat_to_track (test_track, test_beat);
    select_next_beat (test_track);
    if (remove_selected_beat (test_track) != TRACK_STOPPED) {
        return DOES_NOT_MEET_SPEC;
    }
                
    // Test 2: Check if note was removed
    Beat test_beat2 = create_beat();
    add_beat_to_track (test_track, test_beat2);
    Beat test_beat3 = create_beat();
    add_beat_to_track (test_track, test_beat3);
    Beat test_beat4 = create_beat();
    add_beat_to_track (test_track, test_beat4);
    Beat test_beat5 = create_beat();
    add_beat_to_track (test_track, test_beat5);
    select_next_beat (test_track);    
    select_next_beat (test_track);
    if (remove_selected_beat (test_track) != TRACK_PLAYING) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 3: Check if works with last notes
    select_next_beat (test_track);
    if (remove_selected_beat (test_track) != TRACK_STOPPED) {
        return DOES_NOT_MEET_SPEC;
    }

    
//  - What if `remove_selected_beat` doesn't remove a beat?
//  - Does it work differently with the first or last beats?
//  - What if `remove_selected_beat` deletes multiple beats?

    return MEETS_SPEC;
}

// Test function for `add_musical_note_to_beat`
int test_add_musical_note_to_beat(void){
    Beat test_beat = create_beat();

    
    // Test 1:  Does it recognise octaves
    char note[] = "-1A";
    if (add_musical_note_to_beat(test_beat, note) != INVALID_MUSICAL_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2: Does it recognise keys
    char note1[] = "1$";
    if (add_musical_note_to_beat(test_beat, note1) != INVALID_MUSICAL_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 3: Does it work with hashtags
    char note2[] = "1A####%";
    if (add_musical_note_to_beat(test_beat, note2) != INVALID_MUSICAL_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
  
    // Does it even add a note??
    char note3[] = "1A";
    add_musical_note_to_beat(test_beat, note3);
    if (count_notes_in_octave (test_beat, 1) != 1) {
        return DOES_NOT_MEET_SPEC;
    }

    return MEETS_SPEC;
}
