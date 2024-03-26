/**
 * This implementation of IMessageHandler is the first at a simple Text Message
 * Developed By: Chris Hoffman
 * Date: 03/20/2024
 * Release Version: D1.0 
 */

package org.portico.utils.messaging;

import java.util.Map;
import org.portico.utils.messaging.IMessageHandler;
import org.portico.utils.messaging.MessageContext;
import org.portico.lrc.LRCMessageHandler;
import org.portico.lrc.compat.JConfigurationException;
import org.portico2.common.messaging.MessageType;

public class SapphireHandler extends LRCMessageHandler implements IMessageHandler {

	/**
	 * Public No Arg Constructor as recommended by Module.java
	 */
	public SapphireHandler(){}

	public static String name;
	public static int priority_local;


	public void initialize(Map<String,Object> properties) throws JConfigurationException {
		//Inspect the LRC Message Handler for more useful things to do here and how it's relevant to
		//what your doing and why you're doing it.	
		System.out.println("Init: Hello World.");	
	}	

	public void process(MessageContext context ) throws Exception {
		//Inspect MessageContext for methods to call and fields to inspect.
		//Barebones Implementation do nothing.
		//Process the message type Create Federation.
		PorticoMessage myMessage = context.getRequest();
		ResponseMessage myRespone = context.getResponse();

		MessageType decisionType = myMessage.getType();
		//First Exception at CreateFederation is value 3 as defined in MessageType Enum
		switch (decisionType) {
			case CreateFederation:
				System.out.println("getSourceFederate = "+String.valueOf(myMessage.getSourceFederate()));
				System.out.println("getTargetFederate = "+String.valueOf(myMessage.getTargetFederate()));
				System.out.println("getTargetFederation = "+String.valueOf(myMessage.getTargetFederation()));
				System.out.println("getTimeStamp = "+String.valueOf(myMessage.getTimestamp()));
				System.out.println("Is Immediate processing required? "+String.valueOf(myMessage.isImmediateProcessingRequired()));
				break;
		}
	}

	public String getName(){

		if (!this.name.equalsIgnoreCase("")){
			return "EmptyName";
		}
		else
			return this.name;
	}

	public void setName(String name){

		if (!name.equalsIgnoreCase("")){
			this.name = name;
		}
		else {
			this.name = "SapphireStudios";
		}
	}

	public boolean isAugmentable(boolean augmentable){
		if (augmentable == true){
			return true;
		}
		else{
			return false;
		}
	}

	/**
	 * According to IMessageHandler, the value range should be between 1 and 10.
	 * The higher the number the higher the priority of the Augmentable chain of objects 
	 * are linked together, I think we can organize chains of prioritized objects like
	 * player location coordinate objects as higher priority for example. 
	 */
	public int getPriority(){
		return 1;
	}

	public void setPriority( int priority){
		if (priority > 0 && priority <=10){
		this.priority_local = priority;
		}
		else {
			//priority is set to middle range for default setting
			//This will become more clear for other implementations, this one is just for learning purposes.
		this.priority = 5;	
		}

	}
}


