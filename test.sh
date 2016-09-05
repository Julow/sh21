
# Some test

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

touch test;sleep 3&&rm test&while [ -f test ];do sleep 0.5;echo LOOP;done>test2;echo DONE>test2;cat test2

####

for file in `ls`; do printf "[[ %-12s ]]\n" "${file}"; done

####

echo $'\a\b\e\E\f\n\r\t\v\\\'\"' | hd