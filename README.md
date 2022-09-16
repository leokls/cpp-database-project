• Command line database that stores events in a dictionary.

• Supports add, find and delete event, delete date, and print operations. 

• OOP design: the Database class maps Date class to a set of events. 

• Overloaded input/output stream and equality operators.

Usage example:

---
Input:

Add 0-1-2 event1

Add 1-2-3 event2

Find 0-1-2

Del 0-1-2

Print

Del 1-2-3 event2

Del 1-2-3 event2

---
Output:

event1

Deleted 1 events

0001-02-03 event2

Deleted successfully

Event not found
