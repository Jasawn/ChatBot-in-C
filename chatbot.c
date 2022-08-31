/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chat1002.h"


 /*
  * Get the name of the chatbot.
  *
  * Returns: the name of the chatbot as a null-terminated string
  */
const char* chatbot_botname()
{
	return "Bonnie";
}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char* chatbot_username()
{
	return "Clyde";
}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char* inv[], char* response, int n)
{

	/* check for empty input */
	if (inc < 1)  //if word count is 0
	{
		snprintf(response, n, " :(, I am lonely. Chat with me."); //custom response
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function accordingly*/
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);

	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);

	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);

	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);

	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);

	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);

	else
	{
		// If the userinput is not recognised by any of the functions
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}
}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char* intent)
{
	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0; //if user input quit OR exit, return yes
}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char* inv[], char* response, int n)
{
	//Print out goodbye message
	snprintf(response, n, "Goodbye! See you again :)");
	return 1;
}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char* intent)
{
	return compare_token(intent, "load") == 0; //if user input load, return yes
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char* inv[], char* response, int n)
{
	/* Various error messages to be displayed accordingly */
	char* fileerror = "Hey did you miss out something? Please enter a filename!";
	char* overflow = "It is too overwhelming @@";
	char* wrongfileex = "Please enter in <filename.ini> format :')";

	char* filename; //Retrieve the filename that user have input

	errno_t error;

	filename = inv[inc - 1]; //Retrieve the filename that user have input

	if (inc < 2) // Means the user never type in file name
	{
		snprintf(response, n, "%s", fileerror);
		return 0;
	}
	else if (inc > 3) // If user type anything other than load 'filename'
	{
		snprintf(response, n, "%s", overflow);
		return 0;
	}
	else if (!strstr(filename, ".ini")) // If the filename is not .ini
	{
		snprintf(response, n, "%s", wrongfileex);
		return 0;
	}

	FILE* openfile;
	error = fopen_s(&openfile, filename, "r"); // Open the file in read mode

	if (error == 0) // File opened successfully
	{
		int responsenum = knowledge_read(openfile); // Pass the file to knowledge.c
		_fcloseall(); // Close the file after processing

		if (responsenum == KB_NOMEM) // If there is a memory allocation failure
		{
			snprintf(response, n, "%s", overflow);
			return 1;
		}
		else // If no memory allocation failure
		{
			snprintf(response, n, "Read %d responses from %s", responsenum, inv[inc - 1]); // The number of lines read from .ini file
			return 0;
		}
	}
	else // If File did not open successfully
	{
		snprintf(response, n, "File %s is not found", inv[inc - 1]); // Print out the filename that was not found
	}
	return 0;
}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char* intent)
{
	return compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0;
}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char* inv[], char* response, int n)
{
	char entity[MAX_ENTITY] = "";					//the entity
	char intent[MAX_INTENT] = "";					//the intent
	char reply[MAX_RESPONSE] = "";					//reply to the qns
	char* overflow = "It is too overwhelming @@";
	char userinput[MAX_RESPONSE] = "";
	int putvalid = 0;
	int getvalid = 0;
	size_t input = MAX_ENTITY + MAX_RESPONSE + MAX_INTENT + 3;

	if (inc == 1)
	{
		snprintf(response, n, "Sorry, can you please type in complete sentence? :(");
		return 0;
	}
	else if (inc == 2)
	{
		snprintf(response, n, "The sentence is still incomplete :(");
		return 0;
	}

	if (compare_token(inv[1], "is") == 0 || compare_token(inv[1], "are") == 0)
	{
		for (int i = 2; i < inc; i++)
		{
			if (i < inc - 1)
			{
				strcat_s(entity, sizeof(entity), inv[i]);
				strcat_s(entity, sizeof(entity), " ");
			}
			else
			{
				strcat_s(entity, sizeof(entity), inv[i]);
			}
		}
	}
	strcpy_s(intent, sizeof(intent), inv[0]);

	getvalid = knowledge_get(intent, entity, reply, n);
	if (getvalid == KB_OK)
	{
		snprintf(response, n, "%s", reply);
	}
	else if (getvalid == KB_NOTFOUND)
	{
		prompt_user(userinput, input, "I dont know. %s %s %s?\n%s: Tell me more!", intent, inv[1], entity, chatbot_botname());
		if (strcmp(userinput, "") == 0)
		{
			snprintf(response, n, "Type something :( Why you dont wanna tell me..");
		}
		else
		{
			strcat_s(userinput, sizeof(userinput), "\n");
			putvalid = knowledge_put(intent, entity, userinput);

			if (putvalid == KB_OK)
			{
				snprintf(response, n, "Thanks for telling me :).");
			}
			if (putvalid == KB_NOMEM)
			{
				snprintf(response, n, "%s", overflow);
			}
			else if (putvalid == KB_INVALID)
			{
				snprintf(response, n, "Sorry, I cannot understand '%s'.", intent);
			}

		}
	}
	else if (getvalid == KB_INVALID)
	{
		snprintf(response, n, "Sorry, I cannot understand '%s'.", intent);
	}
	return 0;
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char* intent)
{
	return compare_token(intent, "reset") == 0; //if user input reset, return yes
}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char* inv[], char* response, int n)
{
	/* Message to be displayed accordingly */
	char* successful = " reset.";


	knowledge_reset(); // Run the reset function in knowledge.c
	snprintf(response, n, "%s%s", chatbot_botname(), successful); // If the reset is run successfully

	return 0;
}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char* intent) //if user input save, return yes
{
	return compare_token(intent, "save") == 0; //if user input save, return yes
}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char* inv[], char* response, int n)
{
	/* Various error messages to be displayed accordingly */
	char* fileerror = "Hmm, something is missing. Please enter a filename!";
	char* overflow = "It is too overwhelming @@";
	char* wrongfileex = "Please enter in <filename.ini> format :')";
	char* emptylist = "There is nothing to save to file :(";

	/* Get the filename and userinput accordingly */
	char* filename;
	char userinput[MAX_INTENT];
	errno_t error;

	filename = inv[inc - 1]; //Retrieve the filename that user have input

	if (inc < 2) //means the user never type in file name
	{
		snprintf(response, n, "%s", fileerror);
		return 0;
	}
	else if (inc > 3) //if user type anything other than load 'filename'
	{
		snprintf(response, n, "%s", overflow);
		return 0;
	}
	else if (!strstr(filename, ".ini")) // If the filename is not .ini
	{
		snprintf(response, n, "%s", wrongfileex);
		return 0;
	}
	else if (ptrWHO == NULL && ptrWHAT == NULL && ptrWHERE == NULL) // If the linkedlist is empty
	{
		snprintf(response, n, "%s", emptylist);
		return 0;
	}

	FILE* openfile;
	error = fopen_s(&openfile, filename, "r");

	/* Ask the user if they want to continue even if the file exists.. Will overwrite the file*/
	if (error == 0) // File successfully opened
	{
		prompt_user(userinput, n, "\nWARNING: Filename %s exists. Content of the file will be overwritten\nDo you want to proceed saving to %s? [Y/N]", filename, filename);
		if (compare_token(userinput, "y") == 0)
		{
			_fcloseall();
			error = fopen_s(&openfile, filename, "w");
			if (error == 0)
			{
				knowledge_write(openfile);
				snprintf(response, n, "My knowledge has been saved to %s", filename);
				_fcloseall();
				return 0;
			}
			else
			{
				snprintf(response, n, "Error opening the file");
				return 0;
			}
		}
		else if (compare_token(userinput, "n") == 0)
		{
			snprintf(response, n, "Noted :)");
			return 0;
		}
		else
		{
			snprintf(response, n, "I treat it as a No :)");
			return 0;
		}
	}
	else
	{
		_fcloseall();
		error = fopen_s(&openfile, filename, "w");
		if (error == 0)
		{
			knowledge_write(openfile);
			snprintf(response, n, "My knowledge has been saved to %s", filename);
			_fcloseall();
			return 0;
		}
		else
		{
			snprintf(response, n, "Error opening the file");
			return 0;
		}
	}
	return 0;

}


/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char* intent) {

	return compare_token(intent, "it's") == 0 ||
		compare_token(intent, "hi") == 0 ||
		compare_token(intent, "hello") == 0 ||
		compare_token(intent, "hey") == 0 ||
		compare_token(intent, "good") == 0 ||
		compare_token(intent, "tell") == 0 ||
		compare_token(intent, "phrase") == 0 ||
		compare_token(intent, "goodbye") == 0;

}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */



int chatbot_do_smalltalk(int inc, char* inv[], char* response, int n) {

	// Local date and time information
	time_t times = time(NULL);
	struct tm timeinfo;
	localtime_s(&timeinfo, &times);

	int hour = timeinfo.tm_hour;
	int min = timeinfo.tm_min;
	int sec = timeinfo.tm_sec;
	int date = timeinfo.tm_mday;
	int month = timeinfo.tm_mon + 1;
	int year = timeinfo.tm_year + 1900;

	if (compare_token(inv[0], "it's") == 0) {        // check if the first word is it's
		snprintf(response, n, "Indeed it is");        // the chatbot agrees with any statement that begins with "it’s".
	}
	else if (compare_token(inv[0], "hey") == 0 ||
		compare_token(inv[0], "hi") == 0 ||
		compare_token(inv[0], "hello") == 0) {      // check if the first word is hey, hi or hello

		int hres = rand() % 5;              // generate random response (")

		switch (hres) {
		case 0:
			snprintf(response, n, "Yo");
			break;
		case 1:
			snprintf(response, n, "Sup?");
			break;
		case 2:
			snprintf(response, n, "Hello");
			break;
		case 3:
			snprintf(response, n, "It\'s you again");
			break;
		case 4:
			snprintf(response, n, "I\'ve been waiting for you");
			break;
		}
	}
	else if (compare_token(inv[0], "good") == 0) {
		if (hour <= 11) {
			snprintf(response, n, "Good Morning! Today will be a good day!");
			return 0;
		}
		else if (hour > 11 && hour <= 18) {
			snprintf(response, n, "Good Afternoon! What are you doing?");
			return 0;
		}
		else if (hour > 18 && hour <= 21) {
			snprintf(response, n, "Good Evening! How is your day?");
			return 0;
		}
		else if (hour > 21 && hour <= 24) {
			snprintf(response, n, "Good Night \nSleep tight \nDon't let the bedbugs bite!");
			return 1;
		}
	}
	else if (compare_token(inv[0], "tell") == 0) {      // check if the first word is tell

		if (compare_token(inv[inc - 1], "joke") == 0) {    // check if last word is joke

			int tres = rand() % 3;              // generate random response (")

			switch (tres) {
			case 0:
				snprintf(response, n, "Why couldn't the keyboard sleep?\nBecause it has 2 shifts.");
				break;
			case 1:
				snprintf(response, n, "What do you call a sad cup of coffee?\nEspresso");
				break;
			case 2:
				snprintf(response, n, "Why can't you trust trees?\nBecause they are shady");
				break;
			}
		}
		else if (compare_token(inv[inc - 1], "time") == 0) {
			snprintf(response, n, "The time now is %d:%d:%d", hour, min, sec);
			return 0;
		}
		else if (compare_token(inv[inc - 1], "date") == 0) {
			snprintf(response, n, "Today's date is %d/%d/%d", date, month, year);
			return 0;
		}
		else {
			snprintf(response, n, "I am still new and young, my master have only taught me how to tell a joke, the time, and the date.");
		}
	}
	else if (compare_token(inv[0], "phrase") == 0) {      // check if the first word is phrase

		if (compare_token(inv[inc - 1], "day") == 0) {    // check if the last word is day 

			int pres = rand() % 2;              // generate random response (")

			switch (pres) {
			case 0:
				snprintf(response, n, "When life gives you lemon, make a lemonade.");
				break;
			case 1:
				snprintf(response, n, "Your dream doesn\'t have an expration date.\n Take a deep breathe and try again.");
				break;
			}
		}
		else {
			snprintf(response, n, "I am still new and young, my master have only taught me how to give a phrase of the day");
		}
	}
	else if (compare_token(inv[0], "goodbye") == 0) {
		snprintf(response, n, "Goodbye!");
		return 1;                      // stop chatting 
	}

	return 0;

}