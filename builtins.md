# Builtins

### `echo [-neE] [args...]`

Print it's arguments, separated by space

Options:
`-n`		Do not output trailing newline
`-e`		Process some escape sequences:
			`\a`, `\b`, `\E`, `\f`, `\n`, `\r`, `\t`, `\v`, `\\`, `\0`
`-E`		Disable `-e` option

### `true`

Return with a status of 0

### `false`

Return with a status of 1

### `:`

Do nothing (same as `true`)

### `exit [n]`

Exit with a status of `n`

Default value for `n` is the status of the last command (`$?`)

### `pwd`

Print the current working directory

### `set [ARG...]`

TODO: set options

Set positional parameters (`$1`, `$2`, ...)

### `unset [VAR...]`

Unset (remove) a variable

### `shift [n]`

Shift positional parameters

_(all `$i` are set to `${ i + n }`)_

Default value for `n` is `1`
