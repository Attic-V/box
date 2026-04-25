# box

*A tiny C library for drawing boxes around your stdout output.*

**box** is a minimal, dependency-free C library that wraps whatever you print to
`stdout` inside a nicely drawn box. You do not need to modify your print at all.
Simply call `box_begin()` and `box_end()`, and everything in between is
automatically framed.

## Examples

```
#include <stdio.h>

#include <box/box.h>

int main(void)
{
        box_begin();
        printf("hello world");
        box_end();

        box_begin();
        printf("line breaks\nwork too!");
        box_end();

        return 0;
}
```

output:
```
┌─────────────┐
│ hello world │
└─────────────┘
┌─────────────┐
│ line breaks │
│ work too!   │
└─────────────┘
```

*box* also supports nesting:

```
#include <stdio.h>

#include <box/box.h>

int main(void)
{
        box_begin();
        puts("1");

        box_begin();
        puts("2");

        box_begin();
        puts("3");
        box_end();

        box_end();

        box_begin();
        puts("4");
        box_end();

        box_end();

        return 0;
}
```

output:
```
┌───────────┐
│ 1         │
│ ┌───────┐ │
│ │ 2     │ │
│ │ ┌───┐ │ │
│ │ │ 3 │ │ │
│ │ └───┘ │ │
│ └───────┘ │
│ ┌───┐     │
│ │ 4 │     │
│ └───┘     │
└───────────┘
```

## Using with Meson

### As a subproject (recommended)

Add a `subprojects/box.wrap` file to your project:

```
[wrap-git]
directory = box
url = https://github.com/Attic-V/box.git
revision = <commit hash>
```
Then in your `meson.build`:

```
executable('demo', 'main.c', dependencies: [ dependency('box') ])
```
