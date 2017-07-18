using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Slack;
#if WINDOWS_UWP
using System.Threading.Tasks;
using Windows.Networking.Sockets;
using Windows.Storage.Streams;
#else
using WebSocketSharp;
#endif

public class WebSocketConnectionViaSlack : MonoBehaviour
{
    static public WebSocketConnectionViaSlack instance;

    public string token = "0000";
    private string wsURL;

#if WINDOWS_UWP
    MessageWebSocket ws;
#else
    WebSocket ws;
#endif

    public Dictionary<string, wsGetMessageData> latestMessageDict; //userに対応する最新のメッセージを格納

    void Awake()
    {
        if (instance == null)
        {

            instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {

            Destroy(gameObject);
        }

    }

    // Use this for initialization
    void Start()
    {
        latestMessageDict = new Dictionary<string, wsGetMessageData>();
        getConnection(token);
    }

    // Update is called once per frame
    void Update()
    {

    }

    void getConnection(string _token)
    {
        var routine = SlackAPI.getWebSocketURL(_token, connectionSuccess, connectionFailed);
        StartCoroutine(routine);
    }

    void connectionSuccess(rtmConnect connect)
    {
        Debug.Log(connect.url);
        wsURL = connect.url;

        initializeWebSocket();
    }

    void connectionFailed(string error)
    {
        Debug.Log("showErrorMessage: " + error);
    }

    void initializeWebSocket()
    {
        if (wsURL != null)
        {
#if WINDOWS_UWP
            ws = new MessageWebSocket();
            ws.Control.MessageType = SocketMessageType.Utf8;
#else
            ws = new WebSocket(wsURL);
#endif
        }
        else
        {
            Debug.Log("Web Socket intialize failure.");
            return;
        }

#if WINDOWS_UWP
        ws.MessageReceived += Websocket_MessageReceived;

        
        var task = Task.Run(async () => {
            await ws.ConnectAsync(new Uri(wsURL));
        });
        task.Wait();
#else
        ws.OnOpen += onOpen;
        ws.OnMessage += onMessage;
        ws.OnError += onError;
        ws.OnClose += onClose;

        ws.Connect();
#endif
    }

    public void sendMessage(string channelID, string message)
    {
        wsPostMessageData msgData = new wsPostMessageData
        {
            id = Time.time.ToString(), //適当に
            type = "message",
            channel = channelID, //"D4WNU3BK7", //@haruka  "C4WNTPLQM", //random
            text = message
        };

#if WINDOWS_UWP
#else
        ws.Send(JsonUtility.ToJson(msgData));
#endif
    }

    public void updateLatestMessageDict(wsGetMessageData data)
    {
        if(!latestMessageDict.ContainsKey(data.user))
        {
            latestMessageDict.Add(data.user, data);
        }
        else
        {
            latestMessageDict[data.user] = data;
        }
    }

#if WINDOWS_UWP
    void Websocket_MessageReceived(MessageWebSocket sender, MessageWebSocketMessageReceivedEventArgs args)
    {
        //if (OnMessaged != null)
        //{
        var reader = args.GetDataReader();
        reader.UnicodeEncoding = Windows.Storage.Streams.UnicodeEncoding.Utf8;

        string readStr = reader.ReadString(reader.UnconsumedBufferLength);

        Debug.Log(readStr);

        wsGetMessageData msgData = JsonUtility.FromJson<wsGetMessageData>(readStr);

        Debug.Log(msgData.text + " from " + msgData.user + " Type: " + msgData.type);

        if (msgData.type == "message")
        {
            updateLatestMessageDict(msgData);
        }

        //}
    }
#else
    public void onOpen(object sender, System.EventArgs e)
    {
        Debug.Log("WebSocket Open");
    }

    public void onMessage(object sender, MessageEventArgs e)
    {
        //Debug.Log("WebSocket Message Data: " + e.Data);

        wsGetMessageData msgData = JsonUtility.FromJson<wsGetMessageData>(e.Data);

        if (msgData.type == "message")
        {
            updateLatestMessageDict(msgData);
        }

        Debug.Log(msgData.text + " from " + msgData.user + " Type: " + msgData.type);

    }

    public void onError(object sender, ErrorEventArgs e)
    {
        Debug.Log("WebSocket Error Message: " + e.Message);
    }

    public void onClose(object sender, CloseEventArgs e)
    {
        Debug.Log("WebSocket Close");
    }
#endif

    void OnDestroy()
    {
        if (ws != null)
        {
#if WINDOWS_UWP
            ws.Close(1000, "Disconnect function is called.");
#else
            ws.Close();
            ws = null;
#endif
        }
    }
}

