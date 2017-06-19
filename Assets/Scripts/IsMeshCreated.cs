using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using HoloToolkit.Unity.SpatialMapping;

using UnityEngine.UI;

public class IsMeshCreated : MonoBehaviour {

    public Text meshStateTxt;

    public string text = "mesh state...";

    public void Update()
    {
        if(isMeshCreated())
        {
            text = "Mesh has created.";
        }
        else
        {
            text = "Now creating mesh...";
        }
        meshStateTxt.text = text;
    }

    public bool isMeshCreated()
    {
        return SpatialMappingManager.Instance.Source.SurfaceObjects.Count != 0;
    }

}
