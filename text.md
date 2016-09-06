# Text

Text is composed of unquoted words, spaces, strings and substitutions

The `\\` character can be used to escape any other character

# String

### `"str"`

Accept substitutions and `\\"` sequence, escape other characters

Prevent substitutions output from being splitted into multiple arguments

### `'str'`

Anything inside is escaped, do not accept any substitution or escape sequence
_(not even `\\'`)_

### `$'str'`

Like single quoted string but accept some escape sequences:

| | |
|---|---|
| `\\a`, `\\b`, `\\f`, `\\n`, `\\r`, `\\t`, `\\v`	| ansi-c |
| `\\e`, `\\E`					| esc |
| `\\\\`, `\\'`, `\\"`			| `\\`, `'`, `"` |
| `\\Onnn`, `\\onnn`, `\\nnn`	| Octal value; `nnn` is 1 to 3 octal digits |
| `\\Xnn`, `\\xnn`				| Hexadecimal value; `nn` is 1 to 2 hex digits |
| `\\Cn`, `\\cn`				| Control-`n` value; `n` is any letter |

# Substitutions

Result of unquoted substitutions is splitted into multiple arguments
_(using `$IFS` characters)_

### `$(COMMAND)`
### `\`COMMAND\``

Expand to the output of `COMMAND`

### `$PARAM` 

`PARAM` is an indentifier, a single digit or one of `*@#?$!-`

Expand to the value of the variable `PARAM`

If the variable `PARAM` is not set, expand to an empty string

### `${#PARAM}`

Expand to the length of `$PARAM`

### `${PARAM<OP>}`

Expand to `$PARAM` plus:

| `<OP>` | |
|---|---|
| `:-STRING`			| `STRING` is the default value if the variable is unset or null |
| `-STRING`				| `STRING` is the default value if the variable is unset |
| `:=STRING`			| Assign `STRING` to the variable if unset or null |
| `=STRING`				| Assign `STRING` to the variable if unset |
| `:+STRING`			| `STRING` replace value if the variable is set and non-null |
| `+STRING`				| `STRING` replace value if the variable is set |
| `:?STRING`			| Exit with error if the variable is unset or null |
| `?STRING`				| Exit with error if the variable is unset |
| `^`					| Upper case the first character |
| `^^`					| Upper case all characters |
| `,`					| Lower case the first character |
| `,,`					| Lower case all characters |
| `~`					| Reverse case the first character |
| `~~`					| Reverse case all characters |
| `#PATTERN`			| Remove the shortest match of `PATTERN` at the begin of the string |
| `##PATTERN`			| Remove the longest match of `PATTERN` at the begin of the string |
| `%PATTERN`			| Remove the shortest match of `PATTERN` at the end of the string |
| `%%PATTERN`			| Remove the longest match of `PATTERN` at the end of the string |
| `/PATTERN/STRING`		| Replace the first match of `PATTERN` with `STRING` |
| `/%PATTERN/STRING`	| Replace the last match of `PATTERN` with `STRING` |
| `//PATTERN/STRING`	| Replace all matches of `PATTERN` with `STRING` |
| `/PATTERN`			| Remove the first match of `PATTERN` |
| `/%PATTERN`			| Remove the last match of `PATTERN` |
| `//PATTERN`			| Remove all matches of `PATTERN` |
| TODO: `:OFFSET`				| Keep characters from `OFFSET` to the end of the string |
| TODO: `:OFFSET:LENGTH`		| Keep `LENGTH` characters starting from `OFFSET` |

unquoted '*' and '?' in `PATTERN` have special meaning
`PATTERN` and `STRING` can contains more substitutions
`OFFSET` and `LENGTH` are math expression

### TODO: `${!PARAM}`

...

### TODO: `$((EXPR))`

...
