// This file defines a function called dp which will only evaluate its parameters when debug is set to true
#define debug false
#define fps true

#define drun(code) if(debug){code;}
#define frun(code) if(fps){code;}
#define dp(out) drun(dprint(out))
#define ts(comment) drun(timer_start(comment))
#define fps_start(comment) if(fps){timer_start(comment);}
#define te(comment) drun(timer_end(comment, true))

#define text_ovl(src, text, location) putText(src, text, location, FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0, 255, 100))

bool dprint(std::string out);
void timer_start(std::string comment);
float timer_end(std::string comment, bool print);
float fps_end(std::string comment);