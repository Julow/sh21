
# Some test

TEST='ABC/ABC/ABC:% :::ABC:DEF>>!@#IJK<<123'

####

echo $(
	echo b # b
	echo c # c
)

####

for i in a b c ; do
	cat << END ; cat << END
[[[$i]]]
END
{{{$i}}}
END
done

####

for
i
in
a\
b \
c
do
echo \
$i
done>d

for i
in a \
b \
c
do
	echo $i
done 2>d

####

for i in a b c;do echo $i;done

####

while a b; \do c; do d; e; done >f | g

####

if a;b;c;then d;e;f;elif g;h;i;then j;k;l;else m;n;o;fi 2>p | q
if a;b;c;then d;e;f;elif g;h;i;then j;k;l;fi 2>p | q

####

touch /tmp/ko
chmod 0 /tmp/ko
echo a >/tmp/ok 2>/tmp/ko

####

ls | cat
(ls) | cat
ls | cat | cat
ls | (cat) | cat

####

if true; then echo ok; else echo ko; fi
if false; then echo ko; else echo ok; fi

####

touch test; sleep 3 && rm test & while [ -f test ]; do sleep 0.5; echo LOOP; done >test2; echo DONE >test2; cat test2

####

for file in `ls`; do printf "[[ %-12s ]]\n" "${file}"; done

####

echo $'\a\b\e\E\f\n\r\t\v\\\'\"' | hd

####

printf "] %10s [\n" $(ls | cat | rev | sort) | rev

####

{ { echo stdout; echo stderr >&2; } 3>&2 2>&1 >&3; } >stderr 2>stdout
{ echo stdout; echo stderr >&2; } >stdout 2>stderr
echo "stdout -> [$(cat stdout)]" ; echo "stderr -> [$(cat stderr)]"

####

set "a b" "c d" e "f:g"
IFS=:
printf "%s\n" $*
printf "%s\n" $@
printf "%s\n" "$*"
printf "%s\n" "$@"
printf "%s\n" "[$@]"

####

set 1 2 3 4 5 6 7 8 9
printf "[%s] " "$@"; echo "=> $#"; shift

####

function a if [ "$#" -gt 0 ]; then for arg in "$@"; do echo "[${arg}]"; done; else echo "Nothing"; fi
function b { echo "[$@]"; }
function c echo "<$0> #$# [$@]"
function d while [ "$#" -gt 0 ]; do echo "[$1]"; shift; done

#### UNEXPECTED TOKEN

for a in a b c; d
( a; done

#### UNEXPECTED EOF

function
time
!
for a in a b c;

#### UNTERMINATED

# TODO: unterminated strings

` \` ` a
` \` ` a `
` \` ` a ` \`

a \
(
( a
( a;
function lol
a |
a &&
a ||
{
{ a
{ a |
{ a;
for
for a
for a in
for a in a
for a in a b
for a in a b c; do
for a in a b c; do a
for a in a b c; do a;
if
if a
if a; then
if a; then a
if a; then a; elif
if a; then a; elif a
if a; then a; elif a;
if a; then a; elif a; then
if a; then a; elif a; then a
if a; then a; elif a; then a;
if a; then a; elif a; then a; else
if a; then a; elif a; then a; else a
if a; then a; elif a; then a; else a;
if a; then a; elif a; then a; else a; fi

echo a && echo b | cat || echo err ; echo c | cat && true && true && echo d | cat | cat | cat `
