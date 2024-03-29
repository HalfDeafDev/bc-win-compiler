# Brian Callahan's PL/0 on Windows
I am currently working through Brian Callahan's [Let's write a compiler](https://briancallahan.net/blog/20210814.html), a fantastic series of blog posts that covers the foundations of writing a compiler.

He uses C, vim, and OpenBSD during his write up but I've chosen to stick with the same setup, except I'm on Windows.

So far, this hasn't made any major differences, which is pretty awesome. The only difference so far is the use of `strtonum`, which he does conveniently provide in [the source](https://github.com/ibara/pl0c).

But I wanted to stray away from downloading any externals, and instead find the Windows equivalent, which lead to `_strtoll_s`, which returns a `long long` just like [strtonum](https://linux.die.net/man/3/strtonum).

Do I acknowledge that `strtonum` is better? And did I read where it talked about `strtonum` overcoming the shortcomings of `strtol`? Yes, and yes. And if this were anything more than an education piece I'd care more. Instead, I learned about [errno](https://learn.microsoft.com/en-us/cpp/c-runtime-library/errno-doserrno-sys-errlist-and-sys-nerr?view=msvc-170).

Beyond that, I've made changes of my own, opting for a multi-file solution instead of single-file, for no reason other than wanting to separate logic and being tired of working in a single C file all the time.

I've also seemed to run into an issue following the blog post and being able to tokenize any of the provided `.pl0` files from his source that contain the `'` character.

This is probably fine for most people, you can just add the `'` character in to be printed raw or as part of the identity group assuming you're okay with handling the `'`'s elsewhere.

I chose to acknowledge them as their own token, `TOK_STRING`. This modified the grammar slightly, but I don't believe it disqualifies it from validating PL/0 code.

This project will be a mess for a while, at least until I am done with the series, but I should be coming through to clean it up before moving onto building this project again, without the articles, and attempting either a different language, a language of my own, or building out PL/0.
