#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <set>
#include <sstream>
#include <iomanip>

using namespace std;

const string OP_DELIM = " ";
const int OP_DELIM_LEN = 1;
const char DATE_DELIM = '-';
const int DATE_DELIM_LEN = 1;
const string COMMAND_ERR = "Unknown command:";
const string INV_MONTH_VAL = "Month value is invalid:";
const string WRONG_DATE_FORMAT = "Wrong date format:";
const string INV_DAY_VAL = "Day value is invalid:";

class Date {
public:
  Date()
  {
      year = 0;
      month = 0;
      day = 0;
  }
  Date(int y, int m, int d)
  {
      year = y;
      month = m;
      day = d;
  }
  int GetYear() const {
      return year;
  };
  int GetMonth() const {
      return month;
  };
  int GetDay() const {
      return day;
  };
private:
  int year;
  int month;
  int day;
};

bool operator < (const Date &lhs, const Date &rhs) 
{
    if (lhs.GetYear() < rhs.GetYear())                   // 2020-1-1 < 2021-1-1
    {
        return true;
    }
    else if (lhs.GetYear() == rhs.GetYear())
    {
        if (lhs.GetMonth() < rhs.GetMonth())             // 2020-1-1 < 2020-2-1
        {
            return true;
        }
        else if (lhs.GetMonth() == rhs.GetMonth())
        {
            if (lhs.GetDay() < rhs.GetDay())             // 2020-1-1 < 2020-1-2
            {
                return true;
            }
        }
    } 
    return false;
};

string errorMsg(string errMsg, int errVal)
{
    stringstream ss;
    ss << errMsg << ' ' << errVal;
    return ss.str();
}

istream &operator >> (istream &is, Date &date) {

  int year, month, day;
  char delim1, delim2;

  if (is) {
      is >> year >> delim1 >> month >> delim2;
      
      // Check whether the day is missing from the date string.
      if (is.peek() == EOF) {
          throw invalid_argument("");  
      }
      else
      {
          is >> day;      
      }

      // Check whether date has remaining non-numeric characters
      if (is.peek() == EOF) 
      {
          if (delim1 == '-' && delim2 == '-') {
              if (month >= 1 && month <= 12)
              {
                  if (day >= 1 && day <= 31)
                  {
                      date = Date(year, month, day);
                  }
                  // Day value is outside the range.
                  else 
                  {
                      throw runtime_error(errorMsg(INV_DAY_VAL, day));
                  }
              }
              // Month value is outside the range.
              else
              {
                  throw runtime_error(errorMsg(INV_MONTH_VAL, month));
              }              
          }
          // Wrong delimiters.
          else {
              throw invalid_argument("");
          }
      }
      // Missing parts or otherwise incorrect date values.
      else 
      {
          throw invalid_argument("");
      }
  }
  return is;
}

ostream &operator << (ostream &os, const Date &date)
{
    os << setw(4) << setfill('0') << date.GetYear() << '-' 
       << setw(2) << setfill('0') << date.GetMonth() << '-'
       << setw(2) << setfill('0') << date.GetDay();    

    return os;
}

class Database {
public:
    void AddEvent(const Date &date, const string &event)
    {
        db[date].insert(event);
    };
    bool DeleteEvent(const Date &date, const string &event)
    {
        if (db.count(date) > 0) 
        {
            return (bool) db[date].erase(event);    
        }
        else 
        {
            return false;
        }
    };
    int  DeleteDate(const Date &date) 
    {
        int eventsDeleted = 0;
        if (db.count(date) > 0)
        {
            eventsDeleted = db[date].size();
            db.erase(date);
        }
        return eventsDeleted;
    };

    void Find(const Date &date) const {
        
        set <string> setRef = db.at(date);
        vector <string> outputV(setRef.size());
        int idx = 0;
        for (auto & event : setRef)
        {
            outputV[idx] = event;
            ++idx;
        }
        for (auto & event : outputV)
        {
            cout << event << '\n';
        }

    };

    void Print() const {
        for (auto &pair : db)
        {
            // pair.first is Date, pair.second is set <string>.
            for (auto &event : pair.second)
            {
                cout << pair.first << ' ' << event;
                cout << '\n';
            }
        }
    };

    private:
    map <Date, set <string>> db;
};

int main() {
  Database db;
    
  string command;
  while (getline(cin, command)) {

      // Ensure that the op is "Add," "Find," "Del," or "Print". Skip if empty.
      
      // (1) Extract the op from the 'command' string.
      int currPos = command.find(OP_DELIM);
      string op = command.substr(0, currPos);      

      if (op == "") 
      {         
          continue;                                    // Ignore empty strings.
      }
      else if (op == "Print")
      {
          db.Print();
      }
      else if (op == "Add" || op == "Del" || op == "Find")
      {
          // (2) Extract the date from the remaining 'command' string.
          command.erase(0, currPos + OP_DELIM_LEN);
          // find() will return -1 to indicate no match.
          currPos = command.find(OP_DELIM);
          string dateStr = command.substr(0, currPos);
          
          Date date;
          try 
          {
              stringstream ss(dateStr);
              ss >> date;
          } 
          catch (invalid_argument)
          {
              // istream is empty after >> passes it to the Date class.
              cout << "Wrong date format: " << dateStr << '\n';              
              break;                                   // Exit program.
          } 
          catch (runtime_error &ex)
          {
              cout << ex.what() << '\n';
              break;                                   // Exit program. 
          }   

          // (3) Extract the event from the remaining 'command' string.
          // When 'command' contains the 'event' substring.
          string event;
          if (currPos != -1)
          {
              event = command.erase(0, currPos + OP_DELIM_LEN);
          }          

          if (op == "Add")
          {
              db.AddEvent(date, event);  
          }
          else if (op == "Del") 
          {
              if (event == "")
              {
                  cout << "Deleted " << db.DeleteDate(date) << " event(s)." 
                       << '\n';    
              }
              else 
              {
                  if (db.DeleteEvent(date, event))
                  {
                      cout << "Event deleted successfully." << '\n';
                  } 
                  else
                  {
                      cout << "Event not found." << '\n';
                  }
              }
          }
          else if (op == "Find")
          {
              try
              {
                  db.Find(date); 
              }
              catch(out_of_range)
              {
                  continue; 
              }           
          }
      }
      else 
      {
          cout << COMMAND_ERR << " " << op << '\n';
          break;                                       // Exit program.
      }
  }
  return(EXIT_SUCCESS);
}
