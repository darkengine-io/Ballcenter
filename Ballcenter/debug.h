// This file defines a function called dp which will only evaluate its parameters when debug is set to true
#define debug false

#define drun(code) if(debug){code;}
#define dp(out) drun(dprint(out))
#define ts(comment) drun(timer_start(comment))
#define te(comment) drun(timer_end(comment))

bool dprint(std::string out);
void timer_start(std::string comment);
float timer_end(std::string comment);
