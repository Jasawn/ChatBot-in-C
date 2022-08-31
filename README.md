Objective 

Chatbots – computer programs that can respond to queries typed in natural language – have recently 
become a popular addition to web sites and other interactive applications. 
The aim of this project is to implement a simple chatbot that can respond to simple queries made up of 
a single question word (called the intent) and an object (called the entity). The chatbot will be able to 
answer questions such as Where is the printer? or What is C? by identifying the intent (where or what) 
and entity (printer or C) then looking up the answer corresponding to this pair in a simple database. 
The chatbot will also be able to learn new answers by asking questions of the user. If a user asks a 
question for which the database does not contain an answer, the user will be given the option to supply 
an answer that can be used to respond to the same question in future. (This simulates having the chatbot 
refer the unknown question to a third party who does know the answer.) 

Detailed Requirements 
Your team is required to implement, in C, a chatbot of the kind described in the introduction. The 
chatbot will converse in the console using a command-line-like interface; no graphical UI is required.
On each iteration of the main loop of the program (implemented for you in the main function in the 
skeleton code), the program will display a prompt and wait for the user to type a question or instruction. 
Firstly, your chatbot should recognise some stock words and phrases to which it can give rote replies. 
These phrases will be referred to as smalltalk in the remainder of this document. For example, the 
chatbot may reply “hello” whenever the user types “hello” or “hi”. Your chatbot should have at least 
five items of smalltalk. 
Apart from smalltalk, the chatbot will understand only a very simplified form of English in which all 
sentences are composed of only two parts, called the intent and the entity, in that order. 
 The intent represents what the user wants to do or to know. It may be a verb instructing the 
chatbot to do something, or a question word like what, where, etc. 
 The entity is a noun phrase representing the object to which the instruction or question refers. 
The table below describes the intents and entities that must be understood by your chatbot. 
Instructions Entity Description 
RESET - Reset the chatbot to its initial state. 
LOAD filename Load entities and responses from filename. 
SAVE filename Save the known entities and responses to filename. 
EXIT - Exit the program. 
 
Questions Entity Description 
WHERE [IS] any noun phrase Give the location of the entity. 
WHAT [IS] any noun phrase Give a definition of the term. 
WHO [IS] name Describe the person of this name.
For example, the user might type in Where is SIT?, which consists of an intent “WHERE” and an entity 
“SIT”. The chatbot might answer SIT has campuses at each of Singapore’s polytechnics, and a 
dedicated campus at Dover.
The chatbot will start without any knowledge of any entities, but can learn about them by asking the 
user. If someone asks it a question for which it does not know the answer, the chatbot will print I don’t 
know followed by the question. The user may then type in answer. If the user types an answer, the 
chatbot will thereafter user this answer. If the user simply presses ‘enter’ without typing anything, the 
chatbot will ignore the response. In either case, the chatbot will then go back to waiting for more 
questions. 
 A conversation might therefore proceed as follows: 
Chatbot: Hello.
Me: What is the ICT Cluster?
Chatbot: I don’t know. What is the ICT Cluster?
Me: The ICT Cluster teaches information and communications technology.
Chatbot: Thank you.
Me: What is the ICT Cluster?
Chatbot: The ICT Cluster teaches information and communications technology.
Some more detailed sample output is given at the end of this specification sheet. 
Once the chatbot has learned some entities, they can be saved to disk using the SAVE intent, and 
recalled using the LOAD intent. The RESET intent will erase all of the chatbot’s knowledge, leaving 
only the smalltalk in memory (but it will not erase any of the files created by SAVE, so that they can 
be re-loaded later). 
The LOAD intent should append all of the entities and responses in the file to whatever entities and 
responses already exist in the chatbot’s memory. If an entity in the file is the same as one that is already 
in memory, the corresponding response from the file should replace the response currently in memory. 

Specifications
Skeleton 
The skeleton program provided to you gives some guidance on how to structure a program of this kind 
and some hints on how it can be implemented. It consists of a header file, chat1002.h, that contains 
all of the type definitions and function prototypes for the program together with three modules as 
follows: 
 main.c contains the main loop and some utility functions. It prompts the user to type in a 
question, gathers the response, and divides it into words. You should not need to modify this 
file. 
 chatbot.c implements the behaviour of the chatbot. It is required to: 
1. Recognise smalltalk and provide an appropriate response. 
2. Otherwise, identify the intent and entity in each line of input from the user. 
3. If the intent is an instruction, carry it out. 
4. If the intent is a question, search the knowledge base (see below) for an answer. 
5. If the knowledge base does not contain an answer, ask the user for an answer, and add 
the new answer to the knowledge base. 
 knowledge.c implements the chatbot’s knowledge base, as described in the next section. For 
each question intent understood by the chatbot, the knowledge base should support 
1. Searching the knowledge base for an answer corresponding to the entity in the question. 
2. Adding a new entity with a corresponding answer to the question. 
3. Erasing the existing entities and answers. 
A detailed explanation of what each function should do is given in a comment at the start of the function. 
Refer also to the comment at the top of chatbot.c for how that module works. 
You may alter the skeleton code if you wish but do be sure to update any comments in the code so that 
they reflect what the modified code does. 

Knowledge Base
Conceptually, the knowledge base consists of a single list for every question intent understood by the 
chatbot. Each element in the list consists of an entity together with the answer to the question for that 
entity. Each list may be of arbitrary length; in particular it may grow indefinitely as the chatbot learns 
more answers to more questions. It is up to you to decide how these lists should be implemented, and 
your implementation should be described in the report that accompanies your submission. 
You may assume that no entity is longer than MAX_ENTITY characters and that no answer is longer than 
MAX_RESPONSE characters, as defined in chat1002.h. 
Different questions may refer to the same entity with different answers. Where is SIT?, for example, 
might have the answer already described, while What is SIT? might explain that SIT is the Singapore 
Institute of Technology, an autonomous university in Singapore. An entity need not be associated with 
an answer to all question intents; Who is SIT?, for example, need not be understood even if Where is 
SIT? and What is SIT? are. 
Entities may be matched using simple a case-insensitive string matching algorithm, such as the one 
implemented by compare_token() in main.c. You do not need to perform expansion of acronyms, 
stemming, etc. or any other sophisticated types of matching (these require the use of third party libraries 
in C). 

File Format 
A chatbot’s knowledge base will be stored on disk using a file format similar to that used by “INI” files. 
An INI file is a text file divided into sections, with each section containing a series of key-value pairs 
written one per line. For our chatbot, each section corresponds to an intent, and each key-value pair 
specifies an entity (the key) and the answer for the intent with regard to that entity (the value). 
Each section begins with a line containing the name of that section (i.e. the intent) in square brackets. 
Each line within the section consists of the key, followed by a single equal sign (=), followed by the 
corresponding value. You may assume that no line of the file is longer than MAX_ENTITY + 1 +
MAX_RESPONSE + 1 characters, as defined in chat1002.h (the +1’s account for the equal sign and 
the new line character). 
When reading a file, other lines should be handled as follows: 
 blank lines should be ignored 
 if the file contains a section heading that does not correspond to an intent understood by the 
chatbot, the whole section should be ignored 
 any entity/response pairs that occur prior to the first section heading should be ignored 
 lines that do not contain either an equal sign or square brackets should be ignored 
 any characters that appear on the line after the closing square bracket in a section heading 
should be ignored 
