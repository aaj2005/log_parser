# Log Parser

I wanted to brush up on my C++ and return to programming without claude post placement year. Wasn't sure what to do so my natural instincts were to ask Gemini: `spec out a c++ project that i can finish in two days`. Ironically, it gave me a log parser (iykyk) xD


Reads log files with log lines of the following shape (again, Gemini gave me the format xD):

```bash
# IP Address - Timestamp - Request - Status - Bytes sent
192.168.1.5 - - [22/Jul/2026:12:01:00 +0000] "GET /home HTTP/1.1" 200 1024
```

I wrote the code by hand with help from stackoverflow and cppreference reminding me of some of the methods and how they work. Gemini was also useful to help me understand some niche concepts where google resources did not phrase it in a way I could internalise more easily.

It is not the most efficient or versatile log parser but it works, I wrote it by hand, I am decently satisfied.