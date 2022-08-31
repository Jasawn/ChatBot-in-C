/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file. (DONE)
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */


int knowledge_get(const char* intent, const char* entity, char* response, int n)
{
	int match = 0; // Whether entity is found in the linked list

	if (compare_token(intent, "what") == 0) // if the user's question is what
	{
		if (ptrWHAT != NULL) // if the linkedlist is not empty
		{
			getWHAT = ptrWHAT; // Point the looppointer to the head of the linkedlist
			/*int a  = 0*/
			
			while (getWHAT != NULL) // Used for loop ie. a <= linkedlist.length
			{
				if (compare_token(getWHAT->entity, entity) == 0) //Entity matched
				{
					strncpy_s(response, sizeof(getWHAT->response), getWHAT->response, n); // Return the response to show to user
					match++;
					return KB_OK;
				}
				else
				{
					getWHAT = getWHAT->next; // Continue to the next item ie. a++
				}
			}
		}
		return KB_NOTFOUND;
	}

	else if (compare_token(intent, "who") == 0)  // if the user question is what 
	{
		if (ptrWHO != NULL)  // if the link list is not empty 
		{
			getWHO = ptrWHO; // Point the looppointer to the head of the linkedlist
			/*int a  = 0*/
			while (getWHO != NULL) // Used for loop ie. a <= linkedlist.length
			{
				if (compare_token(getWHO->entity, entity) == 0)  // convert all to upper case, then check if the loop can find the entity 
				{
					strncpy_s(response, sizeof(getWHO->response), getWHO->response, n); // Return the response to show to user
					match++;

					return KB_OK;
				}
				else
				{
					getWHO = getWHO->next; // Continue to the next item ie. a++
				}
				
			}
		}
		return KB_NOTFOUND;
	}

	else if (compare_token(intent, "where") == 0)  // if the user question is what 
	{
		if (ptrWHERE != NULL)  // if the link list is not empty 
		{
			getWHERE = ptrWHERE; // Point the looppointer to the head of the linkedlist
			/*int a  = 0*/

			while (getWHERE != NULL) // Used for loop ie. a <= linkedlist.length
			{
				if (compare_token(getWHERE->entity, entity) == 0)  // convert all to upper case, then check if the loop can find the entity 
				{
					strncpy_s(response, sizeof(getWHERE->response), getWHERE->response, n); // Return the response to show to user
					match++;
					return KB_OK;
				}
				else
				{
					getWHERE = getWHERE->next; // Continue to the next item ie. a++
				}
			}
		}
		return KB_NOTFOUND;
	}

	else // if intent is not recognised
	{
		return KB_INVALID;
	}

}

/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) 
{
	int match = 0;
	if (compare_token(intent, "what") == 0) //if intent is what
	{
		RESPONSE* data = malloc(sizeof(RESPONSE)); // Allocate memory for the data to be put into linkedlist

		if (data == NULL)
		{
			return KB_NOMEM; //Memory allocation error
		}

		if (ptrWHAT == NULL) // if there is nothing in linked list
		{
			/* The data below will be the first item of the linked list*/
			strcpy_s(data->intent, MAX_INTENT, intent);
			strcpy_s(data->entity, MAX_ENTITY, entity);
			strcpy_s(data->response, MAX_RESPONSE, response);

			/*printf("%s\n", data->entity);
			printf("%s\n", data->intent);
			printf("%s\n", data->response);*/

			data->next = NULL;
			ptrWHAT = data; //ptrWHAT points to the head of the list
			loopWHAT = data; //loopWHAT allows us to loop through the linked list
		}
		else // if there is items in linked list
		{
			current = loopWHAT; //Save the location of the current loop pointer
			loopWHAT = ptrWHAT; //Point the loop pointer to the beginning

			while (loopWHAT != NULL)
			{
				if (compare_token(loopWHAT->entity,entity) == 0) //If the entity already existed
				{
					strcpy_s(data->response, MAX_RESPONSE, response);
					strcpy_s(loopWHAT->response, MAX_RESPONSE, data->response); //Update the value over
					match++;
				}
				loopWHAT = loopWHAT->next;
			}

			loopWHAT = current; //Point the loop pointer back to the original position
			/* Retrieve the data */

			if (match == 0)
			{
				strcpy_s(data->intent, MAX_INTENT, intent);
				strcpy_s(data->entity, MAX_ENTITY, entity);
				strcpy_s(data->response, MAX_RESPONSE, response);

				/*printf("%s\n", loopWHAT->entity);
				printf("%s\n", loopWHAT->intent);
				printf("%s\n", loopWHAT->response);*/


				data->next = NULL;
				loopWHAT->next = data; // Link the previous item to the current item
				loopWHAT = loopWHAT->next; // Move on to the next node
			}
		}
	}

	else if (compare_token(intent, "who") == 0) //if intent is what
	{
		RESPONSE* data = malloc(sizeof(RESPONSE)); // Allocate memory for the data to be put into linkedlist

		if (data == NULL)
		{
			return KB_NOMEM; //Memory allocation error
		}

		if (ptrWHO == NULL) // if there is nothing in linked list
		{
			/* The data below will be the first item of the linked list*/
			strcpy_s(data->intent, MAX_INTENT, intent);
			strcpy_s(data->entity, MAX_ENTITY, entity);
			strcpy_s(data->response, MAX_RESPONSE, response);

			/*printf("%s\n", data->entity);
			printf("%s\n", data->intent);
			printf("%s\n", data->response);*/

			data->next = NULL;
			ptrWHO = data; //ptrWHAT points to the head of the list
			loopWHO = data; //loopWHAT allows us to loop through the linked list
		}
		else // if there is items in linked list
		{
			current = loopWHO; //Save the location of the current loop pointer
			loopWHO = ptrWHO; //Point the loop pointer to the beginning

			while (loopWHO != NULL)
			{
				if (compare_token(loopWHO->entity, entity) == 0) //If the entity already existed
				{
					strcpy_s(data->response, MAX_RESPONSE, response);
					strcpy_s(loopWHO->response, MAX_RESPONSE, data->response); //Update the value over
					match++;
				}
				loopWHO = loopWHO->next;
			}

			loopWHO = current; //Point the loop pointer back to the original position

			if (match == 0)
			{
				strcpy_s(data->intent, MAX_INTENT, intent);
				strcpy_s(data->entity, MAX_ENTITY, entity);
				strcpy_s(data->response, MAX_RESPONSE, response);

				/*printf("%s\n", loopWHO->entity);
				printf("%s\n", loopWHO->intent);
				printf("%s\n", loopWHO->response);*/

				data->next = NULL;
				loopWHO->next = data; // Link the previous item to the current item
				loopWHO = loopWHO->next; // Move on to the next node
			}
		}
	}

	else if (compare_token(intent, "where") == 0) //if intent is where
	{
		RESPONSE* data = malloc(sizeof(RESPONSE)); // Allocate memory for the data to be put into linkedlist

		if (data == NULL)
		{
			return KB_NOMEM;  // memory allocation failed 
		}

		if (ptrWHERE == NULL) // if there is nothing in the linked list 
		{
			strcpy_s(data->intent, MAX_INTENT, intent);   // eg. what 
			strcpy_s(data->entity, MAX_ENTITY, entity);   // eg. ICT1004 
			strcpy_s(data->response, MAX_RESPONSE, response);   // eg. explanation of ICT1004 

			/*printf("%s\n", data->intent);
			printf("%s\n", data->entity);
			printf("%s\n", data->response);*/

			data->next = NULL;
			ptrWHERE = data; // ptrWHERE is always pointing to the head of the list 
			loopWHERE = data;
		}
		else
		{
			current = loopWHERE; //Save the location of the current loop pointer
			loopWHERE = ptrWHERE; //Point the loop pointer to the beginning

			while (loopWHERE != NULL)
			{
				if (compare_token(loopWHERE->entity, entity) == 0) //If the entity already existed
				{
					strcpy_s(data->response, MAX_RESPONSE, response);
					strcpy_s(loopWHERE->response, MAX_RESPONSE, data->response); //Update the value over
					match++;
				}
				loopWHERE = loopWHERE->next;
			}

			loopWHERE = current; //Point the loop pointer back to the original position

			if (match == 0)
			{
				strcpy_s(data->intent, MAX_INTENT, intent);   // eg. what 
				strcpy_s(data->entity, MAX_ENTITY, entity);   // eg. ICT1004 
				strcpy_s(data->response, MAX_RESPONSE, response);   // eg. explanation of ICT1004 

				/*printf("%s\n", loopWHAT->entity);
				printf("%s\n", loopWHAT->intent);
				printf("%s\n", loopWHAT->response);*/


				// get all the data first 
				data->next = NULL;
				// after get the data point it to the next one 
				loopWHERE->next = data; // Link the previous item to the current item
				loopWHERE = loopWHERE->next; // Move on to the next node
			}
		}
	}
	else
	{
		return KB_INVALID;
	}
	return KB_OK;
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *openfile) 
{
	/* Variables needed for the function*/
	int counter = 0; // Get the number of response in the .ini file
	char* idk = NULL; // IDK
	char* splitstring; 
	char intent[MAX_INTENT], entity[MAX_ENTITY], response[MAX_RESPONSE]; // Set the maxmimum number of characters allowed in each variables

	size_t length = MAX_ENTITY + MAX_RESPONSE + 2;
	char* readstr = malloc(length * sizeof(char)); // Memory allocation

	if (readstr == NULL) // If memory allocation failed
	{
		return KB_NOMEM;
	}
	else
	{
		while (fgets(readstr, length, openfile) != NULL) // Read the file line by line
		{
			if (readstr[0] == '[') // If the sentence consists of square bracket, then it is an Intent
			{
				splitstring = strtok_s(readstr, "[]", &idk); // Remove the square brackets
				strcpy_s(intent, sizeof(intent), splitstring); // Copy the clean string into intent
			}
			if (strstr(readstr, "=")) // If the line consists equal sign, then it is Entity + Response
			{
				splitstring = strtok_s(readstr, "=", &idk); //Get entity
				strcpy_s(entity, sizeof(entity), splitstring); // Copy the clean string into Response
				splitstring = strtok_s(NULL, "=", &idk); //Get the response
				strcpy_s(response, MAX_RESPONSE, splitstring); // Copy the clean string into Response

				counter++;

				knowledge_put(intent, entity, response); // Pass each variables accordingly to put into list
			}
		}
	}
	free(readstr); // Free the memory allocated
	return counter; // Pass back the response retrieved from the file
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() 
{
	if (ptrWHAT != NULL) // if items still exists in linkedlist
	{
		loopWHAT = ptrWHAT;
		while (ptrWHAT != NULL) // if the linked list is still not empty
		{
			loopWHAT = ptrWHAT; // point the looppointer to the first item of the list
			ptrWHAT = ptrWHAT->next;
			free(loopWHAT); // Remove item from linked list
		}
		free(ptrWHAT);
	}

	if (ptrWHO != NULL)   // check if item still exist in the link list 
	{
		loopWHO = ptrWHO;
		while (ptrWHO != NULL)
		{
			loopWHO = ptrWHO; // point the looppointer to the first item of the list
			ptrWHO = ptrWHO->next;
			free(loopWHO); // Remove item from linked list
		}
		free(ptrWHO);
	}

	if (ptrWHERE != NULL)   // check if item still exist in the link list 
	{
		loopWHERE = ptrWHERE;
		while (ptrWHERE != NULL)
		{
			loopWHERE = ptrWHERE; // point the looppointer to the first item of the list
			ptrWHERE = ptrWHERE->next;
			free(loopWHERE); // Remove item from linked list
		}
		free(ptrWHERE);
	}
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) 
{
	size_t length = MAX_ENTITY + MAX_RESPONSE + 2;
	char* intent = malloc(length * sizeof(char)); // Memory allocation

	if (intent == NULL)
	{
		return KB_NOMEM;
	}
	else
	{
		if (ptrWHAT != NULL)
		{
			strcpy_s(intent, sizeof(ptrWHAT->intent), ptrWHAT->intent);
			fprintf(f, "[%s]\n", intent); // Add bracket to intent
			loopWHAT = ptrWHAT;
			while (loopWHAT != NULL) // Entity + Response
			{
				fprintf(f, "%s=", loopWHAT->entity); // Add equal sign after entity
				fprintf(f, "%s", loopWHAT->response);
				loopWHAT = loopWHAT->next;
			}
		}
		fprintf(f, "%s", "\n"); // Add spacing between sections

		if (ptrWHERE != NULL)
		{
			strcpy_s(intent, sizeof(ptrWHERE->intent), ptrWHERE->intent);
			fprintf(f, "[%s]\n", intent); // Add bracket to intent
			loopWHERE = ptrWHERE;
			while (loopWHERE != NULL) // Entity + Response
			{
				fprintf(f, "%s=", loopWHERE->entity); // Add equal sign after entity
				fprintf(f, "%s", loopWHERE->response);
				loopWHERE = loopWHERE->next;
			}
		}
		fprintf(f, "%s", "\n"); // Add spacing between sections

		if (ptrWHO != NULL)
		{
			strcpy_s(intent, sizeof(ptrWHO->intent), ptrWHO->intent);
			fprintf(f, "[%s]\n", intent); // Add bracket to intent
			loopWHO = ptrWHO; 
			while (loopWHO != NULL) // Entity + Response
			{
				fprintf(f, "%s=", loopWHO->entity); // Add equal sign after entity
				fprintf(f, "%s", loopWHO->response);
				loopWHO = loopWHO->next;
			}
		}
		loopWHAT = ptrWHAT;
		loopWHO = ptrWHO;
		loopWHERE = ptrWHERE;
	}
}
