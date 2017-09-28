// Simple date conversions and calculations

#include &lt;Wire.h&gt;
#include &quot;RTClib.h&quot;

void showDate(const char* txt, const DateTime&amp; dt) {
    Serial.print(txt);
    Serial.print(' ');
    Serial.print(dt.year(), DEC);
    Serial.print('/');
    Serial.print(dt.month(), DEC);
    Serial.print('/');
    Serial.print(dt.day(), DEC);
    Serial.print(' ');
    Serial.print(dt.hour(), DEC);
    Serial.print(':');
    Serial.print(dt.minute(), DEC);
    Serial.print(':');
    Serial.print(dt.second(), DEC);
    
    Serial.print(&quot; = &quot;);
    Serial.print(dt.unixtime());
    Serial.print(&quot;s / &quot;);
    Serial.print(dt.unixtime() / 86400L);
    Serial.print(&quot;d since 1970&quot;);
    
    Serial.println();
}

void showTimeSpan(const char* txt, const TimeSpan&amp; ts) {
    Serial.print(txt);
    Serial.print(&quot; &quot;);
    Serial.print(ts.days(), DEC);
    Serial.print(&quot; days &quot;);
    Serial.print(ts.hours(), DEC);
    Serial.print(&quot; hours &quot;);
    Serial.print(ts.minutes(), DEC);
    Serial.print(&quot; minutes &quot;);
    Serial.print(ts.seconds(), DEC);
    Serial.print(&quot; seconds (&quot;);
    Serial.print(ts.totalseconds(), DEC);
    Serial.print(&quot; total seconds)&quot;);
    Serial.println();
}

void setup () {
    Serial.begin(57600);
    
    DateTime dt0 (0, 1, 1, 0, 0, 0);
    showDate(&quot;dt0&quot;, dt0);

    DateTime dt1 (1, 1, 1, 0, 0, 0);
    showDate(&quot;dt1&quot;, dt1);

    DateTime dt2 (2009, 1, 1, 0, 0, 0);
    showDate(&quot;dt2&quot;, dt2);

    DateTime dt3 (2009, 1, 2, 0, 0, 0);
    showDate(&quot;dt3&quot;, dt3);

    DateTime dt4 (2009, 1, 27, 0, 0, 0);
    showDate(&quot;dt4&quot;, dt4);

    DateTime dt5 (2009, 2, 27, 0, 0, 0);
    showDate(&quot;dt5&quot;, dt5);

    DateTime dt6 (2009, 12, 27, 0, 0, 0);
    showDate(&quot;dt6&quot;, dt6);

    DateTime dt7 (dt6.unixtime() + 3600); // One hour later.
    showDate(&quot;dt7&quot;, dt7);

    DateTime dt75 = dt6 + TimeSpan(0, 1, 0, 0); // One hour later with TimeSpan addition.
    showDate(&quot;dt7.5&quot;, dt75);

    DateTime dt8 (dt6.unixtime() + 86400L); // One day later.
    showDate(&quot;dt8&quot;, dt8);

    DateTime dt85 = dt6 + TimeSpan(1, 0, 0, 0); // One day later with TimeSpan addition.
    showDate(&quot;dt8.5&quot;, dt85);

    DateTime dt9 (dt6.unixtime() + 7 * 86400L); // One week later.
    showDate(&quot;dt9&quot;, dt9);

    DateTime dt95 = dt6 + TimeSpan(7, 0, 0, 0); // One week later with TimeSpan addition.
    showDate(&quot;dt9.5&quot;, dt95);

    DateTime dt10 = dt6 + TimeSpan(0, 0, 42, 42); // Fourty two minutes and fourty two seconds later.
    showDate(&quot;dt10&quot;, dt10);

    DateTime dt11 = dt6 - TimeSpan(7, 0, 0, 0);  // One week ago.
    showDate(&quot;dt11&quot;, dt11);

    TimeSpan ts1 = dt6 - dt5;
    showTimeSpan(&quot;dt6-dt5&quot;, ts1);

    TimeSpan ts2 = dt10 - dt6;
    showTimeSpan(&quot;dt10-dt6&quot;, ts2);
}

void loop () {
}
