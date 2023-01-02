package cse3010.lab.edgeDB;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import cse3010.lab.common.CSE3010ReceiverXMLRPC;
import cse3010.lab.common.CSE3010SenderXMLRPC;
import cse3010.lab.common.Message;
import cse3010.lab.common.MessageType;
import cse3010.lab.utils.DebugLog;
import cse3010.lab.utils.ObjectSerializer;

public class CSE3010EdgeDBServer {

	private static CSE3010ReceiverXMLRPC receiver;
	private static String port = "16679";
	
	public String loggerID = "CSE3010EdgeDBServer";
	public String serverID;
	public int serverNo;
	
	private static int clientCnt;
	public static ArrayList<String> clientDevices;

	private static int readCount;
	private static int writeCount;
	private static CSE3010SenderXMLRPC sender;
	private String cloudDBPort = "16689";
	private String cloudDBServerIP = "http://127.0.0.1:" + cloudDBPort + "/xmlrpc";
	private String cloudDBSrvName = "CSE3010 Cloud DB Server";
	private String cloudDBServerReqHandler = "CSE3010CloudDBServer.handleClientRequest";
	
	private static HashMap<String, String> kvStore;
	
	
	/**
	 * This constructor is called multiple times. On client's request, WebServer
	 * creates a CSE3010EdgeDBServer instance on the process of creating a
	 * separate worker to handle the request. (out of our control)
	 * 
	 * Shared variable instantiation should be done in another constructor that
	 * is used for starting up the WebServer
	 * 
	 * NOTE: This should not be called by code that is not WebServer
	 * 
	 * @throws Exception
	 */
	public CSE3010EdgeDBServer() throws Exception {
		if (loggerID == null) {
			this.loggerID = serverID + (serverNo++);
		}
		DebugLog.log("CSE3010EdgeDBServer constructor for each worker thread");
	}
	
	/**
	 * This constructor is to be used to start up the WebServer. This is meant
	 * to be called only once. 
	 * 
	 * @param srvName
	 * @throws Exception
	 */	
	public CSE3010EdgeDBServer(String srvName) {
		serverID = srvName;
		serverNo = 0;
		clientCnt = 0;
		clientDevices = new ArrayList<String>();
		kvStore = new HashMap<String, String>();

		DebugLog.log("cloudDBServerIP=" + cloudDBServerIP);
		sender = new CSE3010SenderXMLRPC(cloudDBServerIP, cloudDBSrvName, cloudDBServerReqHandler);
		readCount = 0;
		writeCount = 0;
	}
	
	public void runServer() {
		receiver = new CSE3010ReceiverXMLRPC(Integer.valueOf(port), "CSE3010EdgeDBServer", this.getClass());
	}

	public void stopServer() {
		receiver.webServer.shutdown();
	}
	
	/*
	 * This function handles Client's request accordingly
	 */
	public Object handleClientRequest(byte[] clReq) throws Exception {

		Message srvmsg = null;
		Object returnObj = null;
		Message cmsg = (Message) ObjectSerializer.deserialize(clReq);
		DebugLog.log("Server has received request for handleClientRequest", this.loggerID);

		Message ack;
		if (!isRegistered(cmsg.senderID) && cmsg.msgType != MessageType.MSG_T_REGISTER_CLIENT) {
			DebugLog.log("Request is from an unregistered client. Reject.");
			ack = new Message();
			ack.msgType = MessageType.MSG_T_NACK;
			ack.msgContent = new String("REGISTER FIRST");
			srvmsg = (Message) ack;
		} else {
			switch (cmsg.msgType) {
			case MessageType.MSG_T_REGISTER_CLIENT:
				DebugLog.log("Register Client Request", this.loggerID);
				returnObj = handleRegisterClient(cmsg);
				ack = new Message();
				ack.msgType = MessageType.MSG_T_ACK;
				ack.msgContent = returnObj;
				srvmsg = (Message) ack;
				break;
			case MessageType.MSG_T_READ_CMD:
				DebugLog.log("Read Request", this.loggerID);
				returnObj = handleReadRequest(cmsg);
				ack = new Message();
				if (returnObj != null) {
					ack.msgType = MessageType.MSG_T_ACK;
				} else {
					ack.msgType = MessageType.MSG_T_NACK;
				}
				ack.msgContent = returnObj;
				srvmsg = (Message) ack;
				break;
			case MessageType.MSG_T_WRITE_CMD:
				DebugLog.log("Write Request", this.loggerID);
				returnObj = handlewriteRequest(cmsg);
				ack = new Message();
				ack.msgType = MessageType.MSG_T_ACK;
				ack.msgContent = returnObj;
				srvmsg = (Message) ack;
				break;
			default:
				break;
			}
		}

		Object retObj = ObjectSerializer.serialize(srvmsg);
		DebugLog.log("Server is responding to a client request", this.loggerID);
		return retObj;
	}
	
	private static String getUniqueCID() {
		return "" + clientCnt++;
	}

	private Object handleRegisterClient(Message cmsg) {
		Message regMsg = new Message();
		regMsg.msgType = MessageType.MSG_T_REGISTER_CLIENT;
		Message srvmsg = null;
		try {
			srvmsg = sender.send(regMsg);
			synchronized (clientDevices) {
				clientDevices.add((String) srvmsg.msgContent);
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println(srvmsg);
		return srvmsg.msgContent;
	}

	private Object handlewriteRequest(Message cmsg) {

		String keyStr = ((String[]) cmsg.msgContent)[0];
		String valStr = ((String[]) cmsg.msgContent)[1];
		String oldVal = kvStore.put(keyStr, valStr);
		// upload key values every 3 writes
		if (++writeCount % 3 == 0) {
			Message writeMsg = new Message();
			writeMsg.msgType = MessageType.MSG_T_WRITE_CMD;
			writeMsg.senderID = cmsg.senderID;
			Iterator<String> iter = kvStore.keySet().iterator();
			String valueToUpdate = null;
			Message srvmsg;
			// iterate through kvStore keys and write their values to the cloud DB
			while (iter.hasNext()) {
				String keyToUpdate = iter.next();
				System.out.println("has key to upload!");
				valueToUpdate = kvStore.get(keyToUpdate);
				String[] writeMsgContent = new String[2];
				writeMsgContent[0] = keyToUpdate;
				writeMsgContent[1] = valueToUpdate;
				writeMsg.msgContent = (Object) writeMsgContent;
				try {
					srvmsg = sender.send(writeMsg);

					//
					if (srvmsg.msgType == MessageType.MSG_T_ACK) {
						DebugLog.log("server responds with ÄCK..", this.loggerID);
					} else {
						// Server NACK..
						DebugLog.elog("SEVERE: Server NACK for some reason.. Exit.", this.loggerID);
						System.exit(1);
					}
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return oldVal;
	}

	private boolean isRegistered(String cid) {
		boolean checkResult = false;
		synchronized(clientDevices) {
			checkResult = clientDevices.contains(cid);
		}
		return checkResult;
	}

	private Object handleReadRequest(Message cmsg) {
		String keyStr = (String) cmsg.msgContent;
		String valStr = null;

		if (++readCount % 3 == 0) {
			Message readMsg = new Message();
			readMsg.msgType = MessageType.MSG_T_READ_KEY_LIST;
			readMsg.senderID = cmsg.senderID;
			try {
				Message srvmsg = sender.send(readMsg);
				if (srvmsg.msgType == MessageType.MSG_T_ACK) {
					DebugLog.log("server responds with ACK.", this.loggerID);
					Message ackmsg = (Message) srvmsg;

					List<String> keysToRead = (List<String>) ackmsg.msgContent;

					DebugLog.log("Read keysToRead from Cloud: keysToRead-" + keysToRead.toString());
					readMsg.msgType = MessageType.MSG_T_READ_CMD;
					for (String key : keysToRead) {
						readMsg.msgContent = (Object) key;
						srvmsg = sender.send(readMsg);

						if (srvmsg.msgType == MessageType.MSG_T_ACK) {
							String newValue = (String) srvmsg.msgContent;
							kvStore.put(key, newValue);
						} else {
							// Server NACK.
							DebugLog.elog("Server NACK: For Unknown Reason.", this.loggerID);
							System.exit(1);
						}
					}
				} else {
					// Server NACK...
					DebugLog.elog("Server NACK: For Unknown Reason.", this.loggerID);
					System.exit(1);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}

		}
		// At this point, all keys have the up-to-date values.
		valStr = kvStore.get(keyStr);
		return valStr;
	}

	
	public static void main(String[] argv) {
		System.out.println("enter the port number for the Edge DB Server:");
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		try {
			String input = br.readLine();
			port = input;
			System.out.println("port number for this Edge DB servers" + port);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		CSE3010EdgeDBServer server = new CSE3010EdgeDBServer("standalone-CSE3010EdgeDBServerEngine-main");
		server.runServer();
	}
}
