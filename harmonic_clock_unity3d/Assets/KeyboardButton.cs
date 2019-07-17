using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class KeyboardButton : MonoBehaviour, IPointerDownHandler, IPointerUpHandler
{

 //   [SerializeField] Button button;
    [SerializeField] Text text;
    //public KeyCode KeyCode;
    [SerializeField] string key;
    public MIDI.Note note;
    string lowerCaseKey;
    Color startColor;
    public Color pressedColor = Color.black;
    Image image;
    void OnValidate()
    {
        if (text == null) text = GetComponentInChildren<Text>();
      //  if (button == null) button = GetComponentInChildren<Button>();
      //  if (text != null) key = text.text;
       // pressedColor=Color.gray;
    }
    void Start()
    {
        image = GetComponent<Image>();
        startColor = image.color;
        lowerCaseKey = key.ToLower();
    }
    public void OnPointerDown(PointerEventData e)
    {
        OnKeyPressed();
    }
    public void OnPointerUp(PointerEventData e)
    {
        OnKeyReleased();
    }
    void OnKeyPressed()
    {
        MIDI.OnNoteOnRecieved((int)note);
//button.interactable = false;
        image.color = pressedColor;
    }
    void OnKeyReleased()
    {
        MIDI.OnNoteOffRecieved((int)note);
       //button.interactable = true;
        image.color = startColor;
    }
    void Update()
    {
        if (Input.GetKeyDown(lowerCaseKey))
            OnKeyPressed();
        else
          if (Input.GetKeyUp(lowerCaseKey))
            OnKeyReleased();
    }
}
