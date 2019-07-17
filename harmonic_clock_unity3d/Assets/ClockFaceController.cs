using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

//attach this to your center dot
public class ClockFaceController : MonoBehaviour
{

    public int rowCount = 2;
    public Image[] ringTemplates;
    List<List<Image>> imageLeds = new List<List<Image>>();
    public Gradient colors;

    void OnValidate()
    {
        if (rowCount < 1) rowCount = 1;
        if (ringTemplates == null || ringTemplates.Length != rowCount) ringTemplates = new Image[rowCount];
        for (int i = 0; i < ringTemplates.Length; i++)
        {
            if (ringTemplates[i] != null)
            {
                var pos = ringTemplates[i].transform.localPosition;
                pos.x = 0;
                ringTemplates[i].transform.localPosition = pos;
            }
        }
    }
    void Start()
    {
        Quaternion rotation = Quaternion.Euler(0, 0, 360 / 12);
        for (int j = 0; j < rowCount; j++)
        {
            imageLeds.Add(new List<Image>());
            imageLeds[j].Add(ringTemplates[j]);
            for (int i = 0; i < 12; i++)
            {
                var thisInstance = Instantiate(ringTemplates[j], transform);
                var pos = ringTemplates[j].transform.localPosition;
                float height = pos.y;
                Vector3 newPos = new Vector3(height * Mathf.Sin(Mathf.Deg2Rad * 360 / 12 * i),
                                             height * Mathf.Cos(Mathf.Deg2Rad * 360 / 12 * i));
                thisInstance.transform.localPosition = newPos;
                imageLeds[j].Add(thisInstance);
            }
        }
    }

}
