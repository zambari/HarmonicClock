using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MIDI : MonoBehaviour
{

    public enum Note { C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B }
    public static void OnNoteOnRecieved(int NoteNumber)
    {
        Debug.Log("note on recieved " + (MIDI.Note)NoteNumber);
    }
    public static void OnNoteOffRecieved(int NoteNumber)
    {
        Debug.Log("note off recieved " + (MIDI.Note)NoteNumber);
    }

}
