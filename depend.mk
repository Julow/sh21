INCLUDE_FLAGS += -I$(O_DIR)/_public
LINK_FLAGS += -ltermcap
OBJ_DIR_TREE += $(O_DIR)/srcs/range/ $(O_DIR)/srcs/main/ \
	$(O_DIR)/srcs/editor/binding/ $(O_DIR)/srcs/editor/ $(O_DIR)/srcs/ \
	$(O_DIR)/libft/ft_term/ $(O_DIR)/libft/ft_printf/formats/ \
	$(O_DIR)/libft/ft_printf/ $(O_DIR)/libft/ft_out/ $(O_DIR)/libft/ft_getkey/ \
	$(O_DIR)/libft/ft_dstr/ $(O_DIR)/libft/ft_bst/ $(O_DIR)/libft/ft_base/ \
	$(O_DIR)/libft/ $(O_DIR)/_public/ft/ $(O_DIR)/_public/ $(O_DIR)/
O_FILES += $(O_DIR)/srcs/editor/binding/cursor_move.o \
	$(O_DIR)/srcs/editor/binding/delete.o $(O_DIR)/srcs/editor/binding_utils.o \
	$(O_DIR)/srcs/editor/editor_bind.o $(O_DIR)/srcs/editor/editor_init.o \
	$(O_DIR)/srcs/editor/editor_key.o $(O_DIR)/srcs/editor/editor_put.o \
	$(O_DIR)/srcs/editor/word_range.o $(O_DIR)/libft/ft_base/ft_abs.o \
	$(O_DIR)/libft/ft_base/ft_assert.o $(O_DIR)/libft/ft_base/ft_atoib.o \
	$(O_DIR)/libft/ft_base/ft_bzero.o $(O_DIR)/libft/ft_base/ft_clock.o \
	$(O_DIR)/libft/ft_base/ft_emalloc.o $(O_DIR)/libft/ft_base/ft_escape.o \
	$(O_DIR)/libft/ft_base/ft_getenv.o $(O_DIR)/libft/ft_base/ft_is.o \
	$(O_DIR)/libft/ft_base/ft_max.o $(O_DIR)/libft/ft_base/ft_memcmp.o \
	$(O_DIR)/libft/ft_base/ft_memcpy.o $(O_DIR)/libft/ft_base/ft_memdup.o \
	$(O_DIR)/libft/ft_base/ft_memfill.o $(O_DIR)/libft/ft_base/ft_memmove.o \
	$(O_DIR)/libft/ft_base/ft_memrcpy.o $(O_DIR)/libft/ft_base/ft_memset.o \
	$(O_DIR)/libft/ft_base/ft_memstart.o $(O_DIR)/libft/ft_base/ft_memswap.o \
	$(O_DIR)/libft/ft_base/ft_min.o $(O_DIR)/libft/ft_base/ft_nanoclock.o \
	$(O_DIR)/libft/ft_base/ft_rand.o $(O_DIR)/libft/ft_base/ft_randbool.o \
	$(O_DIR)/libft/ft_base/ft_strchri.o $(O_DIR)/libft/ft_base/ft_strlen.o \
	$(O_DIR)/libft/ft_base/ft_sub.o $(O_DIR)/libft/ft_base/ft_unescape.o \
	$(O_DIR)/libft/ft_base/ft_widetoa.o $(O_DIR)/libft/ft_base/ft_wstrconv.o \
	$(O_DIR)/libft/ft_base/ft_wstrlen.o $(O_DIR)/libft/ft_base/ft_wstrnconv.o \
	$(O_DIR)/libft/ft_bst/bst_balance.o $(O_DIR)/libft/ft_bst/ft_bstdel.o \
	$(O_DIR)/libft/ft_bst/ft_bstget.o $(O_DIR)/libft/ft_bst/ft_bstiter.o \
	$(O_DIR)/libft/ft_bst/ft_bstmax.o $(O_DIR)/libft/ft_bst/ft_bstmin.o \
	$(O_DIR)/libft/ft_bst/ft_bstput.o $(O_DIR)/libft/ft_dstr/ft_dstradd.o \
	$(O_DIR)/libft/ft_dstr/ft_dstrclear.o \
	$(O_DIR)/libft/ft_dstr/ft_dstrextend.o \
	$(O_DIR)/libft/ft_dstr/ft_dstrspan.o $(O_DIR)/libft/ft_getkey/ft_getkey.o \
	$(O_DIR)/libft/ft_getkey/get_seq.o $(O_DIR)/libft/ft_getkey/next_char.o \
	$(O_DIR)/libft/ft_out/ft_putchar.o $(O_DIR)/libft/ft_out/ft_putendl.o \
	$(O_DIR)/libft/ft_out/ft_putfloat.o $(O_DIR)/libft/ft_out/ft_putint.o \
	$(O_DIR)/libft/ft_out/ft_putnchar.o $(O_DIR)/libft/ft_out/ft_putpad.o \
	$(O_DIR)/libft/ft_out/ft_putstr.o $(O_DIR)/libft/ft_out/ft_putsub.o \
	$(O_DIR)/libft/ft_out/ft_putuint.o $(O_DIR)/libft/ft_out/ft_str_out.o \
	$(O_DIR)/libft/ft_out/ft_write.o $(O_DIR)/libft/ft_out/ft_write_char.o \
	$(O_DIR)/libft/ft_out/ft_write_nchar.o $(O_DIR)/libft/ft_out/out_formats.o \
	$(O_DIR)/libft/ft_printf/args_utils.o \
	$(O_DIR)/libft/ft_printf/exec_format.o \
	$(O_DIR)/libft/ft_printf/exec_subformat.o \
	$(O_DIR)/libft/ft_printf/formats/char.o \
	$(O_DIR)/libft/ft_printf/formats/int.o \
	$(O_DIR)/libft/ft_printf/formats/misc.o \
	$(O_DIR)/libft/ft_printf/formats/str.o $(O_DIR)/libft/ft_printf/ft_out.o \
	$(O_DIR)/libft/ft_printf/ft_printf.o $(O_DIR)/libft/ft_printf/ft_putf.o \
	$(O_DIR)/libft/ft_printf/ft_vprintf.o $(O_DIR)/srcs/range/ft_range.o \
	$(O_DIR)/libft/ft_term/ft_tclear.o $(O_DIR)/libft/ft_term/ft_tcursor.o \
	$(O_DIR)/libft/ft_term/ft_tinit.o $(O_DIR)/libft/ft_term/ft_tput.o \
	$(O_DIR)/libft/ft_term/ft_trestore.o $(O_DIR)/libft/ft_term/ft_tupdate.o \
	$(O_DIR)/libft/ft_term/term_out_flush.o $(O_DIR)/srcs/main/main.o
PUBLIC_LINKS += $(O_DIR)/_public/editor.h $(O_DIR)/_public/editor_bindings.h \
	$(O_DIR)/_public/ft/ft_colors.h $(O_DIR)/_public/ft/ft_wchar.h \
	$(O_DIR)/_public/ft/libft.h $(O_DIR)/_public/ft/ft_bst.h \
	$(O_DIR)/_public/ft/ft_dstr.h $(O_DIR)/_public/ft/getkey.h \
	$(O_DIR)/_public/ft/ft_out.h $(O_DIR)/_public/ft/ft_str_out.h \
	$(O_DIR)/_public/ft/ft_printf.h $(O_DIR)/_public/ft/ft_vprintf.h \
	$(O_DIR)/_public/ft/range.h $(O_DIR)/_public/ft/term.h

# module editor
$(O_DIR)/srcs/editor/binding/cursor_move.o: srcs/editor/binding/cursor_move.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_getkey/public/getkey.h \
	libft/ft_out/public/ft_out.h srcs/editor/editor_internal.h \
	srcs/editor/public/editor.h srcs/editor/public/editor_bindings.h \
	srcs/range/public/range.h
$(O_DIR)/srcs/editor/binding/delete.o: srcs/editor/binding/delete.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_getkey/public/getkey.h \
	libft/ft_out/public/ft_out.h srcs/editor/editor_internal.h \
	srcs/editor/public/editor.h srcs/editor/public/editor_bindings.h \
	srcs/range/public/range.h
$(O_DIR)/srcs/editor/binding_utils.o: srcs/editor/binding_utils.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_getkey/public/getkey.h \
	libft/ft_out/public/ft_out.h srcs/editor/editor_internal.h \
	srcs/editor/public/editor.h srcs/editor/public/editor_bindings.h \
	srcs/range/public/range.h
$(O_DIR)/srcs/editor/editor_bind.o: srcs/editor/editor_bind.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_getkey/public/getkey.h \
	libft/ft_out/public/ft_out.h srcs/editor/editor_internal.h \
	srcs/editor/public/editor.h srcs/range/public/range.h
$(O_DIR)/srcs/editor/editor_init.o: srcs/editor/editor_init.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_getkey/public/getkey.h \
	libft/ft_out/public/ft_out.h srcs/editor/editor_internal.h \
	srcs/editor/public/editor.h srcs/editor/public/editor_bindings.h \
	srcs/range/public/range.h
$(O_DIR)/srcs/editor/editor_key.o: srcs/editor/editor_key.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_getkey/public/getkey.h \
	libft/ft_out/public/ft_out.h srcs/editor/editor_internal.h \
	srcs/editor/public/editor.h srcs/range/public/range.h
$(O_DIR)/srcs/editor/editor_put.o: srcs/editor/editor_put.c \
	libft/ft_base/public/ft_colors.h libft/ft_base/public/libft.h \
	libft/ft_bst/public/ft_bst.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_getkey/public/getkey.h libft/ft_out/public/ft_out.h \
	srcs/editor/editor_internal.h srcs/editor/public/editor.h \
	srcs/range/public/range.h
$(O_DIR)/srcs/editor/word_range.o: srcs/editor/word_range.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_getkey/public/getkey.h \
	libft/ft_out/public/ft_out.h srcs/editor/public/editor.h \
	srcs/range/public/range.h

$(O_DIR)/srcs/editor/binding/cursor_move.o \
$(O_DIR)/srcs/editor/binding/delete.o $(O_DIR)/srcs/editor/binding_utils.o \
$(O_DIR)/srcs/editor/editor_bind.o $(O_DIR)/srcs/editor/editor_init.o \
$(O_DIR)/srcs/editor/editor_key.o $(O_DIR)/srcs/editor/editor_put.o \
$(O_DIR)/srcs/editor/word_range.o: INCLUDE_FLAGS += -Isrcs/editor

# module ft::base
$(O_DIR)/libft/ft_base/ft_abs.o: libft/ft_base/ft_abs.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_assert.o: libft/ft_base/ft_assert.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_atoib.o: libft/ft_base/ft_atoib.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_bzero.o: libft/ft_base/ft_bzero.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_clock.o: libft/ft_base/ft_clock.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_emalloc.o: libft/ft_base/ft_emalloc.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_escape.o: libft/ft_base/ft_escape.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_getenv.o: libft/ft_base/ft_getenv.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_is.o: libft/ft_base/ft_is.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_max.o: libft/ft_base/ft_max.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memcmp.o: libft/ft_base/ft_memcmp.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memcpy.o: libft/ft_base/ft_memcpy.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memdup.o: libft/ft_base/ft_memdup.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memfill.o: libft/ft_base/ft_memfill.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memmove.o: libft/ft_base/ft_memmove.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memrcpy.o: libft/ft_base/ft_memrcpy.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memset.o: libft/ft_base/ft_memset.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memstart.o: libft/ft_base/ft_memstart.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memswap.o: libft/ft_base/ft_memswap.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_min.o: libft/ft_base/ft_min.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_nanoclock.o: libft/ft_base/ft_nanoclock.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_rand.o: libft/ft_base/ft_rand.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_randbool.o: libft/ft_base/ft_randbool.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_strchri.o: libft/ft_base/ft_strchri.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_strlen.o: libft/ft_base/ft_strlen.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_sub.o: libft/ft_base/ft_sub.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_unescape.o: libft/ft_base/ft_unescape.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_widetoa.o: libft/ft_base/ft_widetoa.c \
	libft/ft_base/public/ft_wchar.h libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_wstrconv.o: libft/ft_base/ft_wstrconv.c \
	libft/ft_base/public/ft_wchar.h libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_wstrlen.o: libft/ft_base/ft_wstrlen.c \
	libft/ft_base/public/ft_wchar.h libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_wstrnconv.o: libft/ft_base/ft_wstrnconv.c \
	libft/ft_base/public/ft_wchar.h libft/ft_base/public/libft.h

# module ft::bst
$(O_DIR)/libft/ft_bst/bst_balance.o: libft/ft_bst/bst_balance.c \
	libft/ft_base/public/libft.h libft/ft_bst/bst_internal.h \
	libft/ft_bst/public/ft_bst.h
$(O_DIR)/libft/ft_bst/ft_bstdel.o: libft/ft_bst/ft_bstdel.c \
	libft/ft_base/public/libft.h libft/ft_bst/bst_internal.h \
	libft/ft_bst/public/ft_bst.h
$(O_DIR)/libft/ft_bst/ft_bstget.o: libft/ft_bst/ft_bstget.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h
$(O_DIR)/libft/ft_bst/ft_bstiter.o: libft/ft_bst/ft_bstiter.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h
$(O_DIR)/libft/ft_bst/ft_bstmax.o: libft/ft_bst/ft_bstmax.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h
$(O_DIR)/libft/ft_bst/ft_bstmin.o: libft/ft_bst/ft_bstmin.c \
	libft/ft_base/public/libft.h libft/ft_bst/public/ft_bst.h
$(O_DIR)/libft/ft_bst/ft_bstput.o: libft/ft_bst/ft_bstput.c \
	libft/ft_base/public/libft.h libft/ft_bst/bst_internal.h \
	libft/ft_bst/public/ft_bst.h

$(O_DIR)/libft/ft_bst/bst_balance.o $(O_DIR)/libft/ft_bst/ft_bstdel.o \
$(O_DIR)/libft/ft_bst/ft_bstget.o $(O_DIR)/libft/ft_bst/ft_bstiter.o \
$(O_DIR)/libft/ft_bst/ft_bstmax.o $(O_DIR)/libft/ft_bst/ft_bstmin.o \
$(O_DIR)/libft/ft_bst/ft_bstput.o: INCLUDE_FLAGS += -Ilibft/ft_bst

# module ft::dstr
$(O_DIR)/libft/ft_dstr/ft_dstradd.o: libft/ft_dstr/ft_dstradd.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h
$(O_DIR)/libft/ft_dstr/ft_dstrclear.o: libft/ft_dstr/ft_dstrclear.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h
$(O_DIR)/libft/ft_dstr/ft_dstrextend.o: libft/ft_dstr/ft_dstrextend.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h
$(O_DIR)/libft/ft_dstr/ft_dstrspan.o: libft/ft_dstr/ft_dstrspan.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h

# module ft::getkey
$(O_DIR)/libft/ft_getkey/ft_getkey.o: libft/ft_getkey/ft_getkey.c \
	libft/ft_base/public/libft.h libft/ft_getkey/getkey_internal.h \
	libft/ft_getkey/public/getkey.h
$(O_DIR)/libft/ft_getkey/get_seq.o: libft/ft_getkey/get_seq.c \
	libft/ft_base/public/libft.h libft/ft_getkey/getkey_internal.h \
	libft/ft_getkey/public/getkey.h
$(O_DIR)/libft/ft_getkey/next_char.o: libft/ft_getkey/next_char.c \
	libft/ft_base/public/libft.h libft/ft_getkey/getkey_internal.h \
	libft/ft_getkey/public/getkey.h

$(O_DIR)/libft/ft_getkey/ft_getkey.o $(O_DIR)/libft/ft_getkey/get_seq.o \
$(O_DIR)/libft/ft_getkey/next_char.o: INCLUDE_FLAGS += -Ilibft/ft_getkey

# module ft::out
$(O_DIR)/libft/ft_out/ft_putchar.o: libft/ft_out/ft_putchar.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_putendl.o: libft/ft_out/ft_putendl.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_putfloat.o: libft/ft_out/ft_putfloat.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_putint.o: libft/ft_out/ft_putint.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_putnchar.o: libft/ft_out/ft_putnchar.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_putpad.o: libft/ft_out/ft_putpad.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_putstr.o: libft/ft_out/ft_putstr.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_putsub.o: libft/ft_out/ft_putsub.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_putuint.o: libft/ft_out/ft_putuint.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_str_out.o: libft/ft_out/ft_str_out.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_out/public/ft_str_out.h
$(O_DIR)/libft/ft_out/ft_write.o: libft/ft_out/ft_write.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_write_char.o: libft/ft_out/ft_write_char.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/ft_write_nchar.o: libft/ft_out/ft_write_nchar.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h
$(O_DIR)/libft/ft_out/out_formats.o: libft/ft_out/out_formats.c \
	libft/ft_base/public/libft.h libft/ft_out/out_internal.h \
	libft/ft_out/public/ft_out.h

$(O_DIR)/libft/ft_out/ft_putchar.o $(O_DIR)/libft/ft_out/ft_putendl.o \
$(O_DIR)/libft/ft_out/ft_putfloat.o $(O_DIR)/libft/ft_out/ft_putint.o \
$(O_DIR)/libft/ft_out/ft_putnchar.o $(O_DIR)/libft/ft_out/ft_putpad.o \
$(O_DIR)/libft/ft_out/ft_putstr.o $(O_DIR)/libft/ft_out/ft_putsub.o \
$(O_DIR)/libft/ft_out/ft_putuint.o $(O_DIR)/libft/ft_out/ft_str_out.o \
$(O_DIR)/libft/ft_out/ft_write.o $(O_DIR)/libft/ft_out/ft_write_char.o \
$(O_DIR)/libft/ft_out/ft_write_nchar.o $(O_DIR)/libft/ft_out/out_formats.o: \
	INCLUDE_FLAGS += -Ilibft/ft_out

# module ft::printf
$(O_DIR)/libft/ft_printf/args_utils.o: libft/ft_printf/args_utils.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/exec_format.o: libft/ft_printf/exec_format.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/exec_subformat.o: libft/ft_printf/exec_subformat.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_out/public/ft_str_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/formats/char.o: libft/ft_printf/formats/char.c \
	libft/ft_base/public/ft_wchar.h libft/ft_base/public/libft.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/formats/int.o: libft/ft_printf/formats/int.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/formats/misc.o: libft/ft_printf/formats/misc.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/formats/str.o: libft/ft_printf/formats/str.c \
	libft/ft_base/public/ft_wchar.h libft/ft_base/public/libft.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_out.o: libft/ft_printf/ft_out.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_printf.o: libft/ft_printf/ft_printf.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_putf.o: libft/ft_printf/ft_putf.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_vprintf.o: libft/ft_printf/ft_vprintf.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h

$(O_DIR)/libft/ft_printf/args_utils.o $(O_DIR)/libft/ft_printf/exec_format.o \
$(O_DIR)/libft/ft_printf/exec_subformat.o \
$(O_DIR)/libft/ft_printf/formats/char.o $(O_DIR)/libft/ft_printf/formats/int.o \
$(O_DIR)/libft/ft_printf/formats/misc.o $(O_DIR)/libft/ft_printf/formats/str.o \
$(O_DIR)/libft/ft_printf/ft_out.o $(O_DIR)/libft/ft_printf/ft_printf.o \
$(O_DIR)/libft/ft_printf/ft_putf.o $(O_DIR)/libft/ft_printf/ft_vprintf.o: \
	INCLUDE_FLAGS += -Ilibft/ft_printf

# module ft::range
$(O_DIR)/srcs/range/ft_range.o: srcs/range/ft_range.c \
	libft/ft_base/public/libft.h srcs/range/public/range.h

# module ft::term
$(O_DIR)/libft/ft_term/ft_tclear.o: libft/ft_term/ft_tclear.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h
$(O_DIR)/libft/ft_term/ft_tcursor.o: libft/ft_term/ft_tcursor.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_term/term_internal.h
$(O_DIR)/libft/ft_term/ft_tinit.o: libft/ft_term/ft_tinit.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_out/public/ft_str_out.h libft/ft_term/public/term.h \
	libft/ft_term/term_internal.h
$(O_DIR)/libft/ft_term/ft_tput.o: libft/ft_term/ft_tput.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_term/term_internal.h
$(O_DIR)/libft/ft_term/ft_trestore.o: libft/ft_term/ft_trestore.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h
$(O_DIR)/libft/ft_term/ft_tupdate.o: libft/ft_term/ft_tupdate.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h
$(O_DIR)/libft/ft_term/term_out_flush.o: libft/ft_term/term_out_flush.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_term/term_internal.h

$(O_DIR)/libft/ft_term/ft_tclear.o $(O_DIR)/libft/ft_term/ft_tcursor.o \
$(O_DIR)/libft/ft_term/ft_tinit.o $(O_DIR)/libft/ft_term/ft_tput.o \
$(O_DIR)/libft/ft_term/ft_trestore.o $(O_DIR)/libft/ft_term/ft_tupdate.o \
$(O_DIR)/libft/ft_term/term_out_flush.o: INCLUDE_FLAGS += -Ilibft/ft_term

# module sh21
$(O_DIR)/srcs/main/main.o: srcs/main/main.c libft/ft_base/public/libft.h \
	libft/ft_bst/public/ft_bst.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_getkey/public/getkey.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/public/ft_printf.h libft/ft_term/public/term.h \
	srcs/editor/public/editor.h srcs/range/public/range.h

# public links
$(O_DIR)/_public/editor.h: srcs/editor/public/editor.h
$(O_DIR)/_public/editor_bindings.h: srcs/editor/public/editor_bindings.h
$(O_DIR)/_public/ft/ft_bst.h: libft/ft_bst/public/ft_bst.h
$(O_DIR)/_public/ft/ft_colors.h: libft/ft_base/public/ft_colors.h
$(O_DIR)/_public/ft/ft_dstr.h: libft/ft_dstr/public/ft_dstr.h
$(O_DIR)/_public/ft/ft_out.h: libft/ft_out/public/ft_out.h
$(O_DIR)/_public/ft/ft_printf.h: libft/ft_printf/public/ft_printf.h
$(O_DIR)/_public/ft/ft_str_out.h: libft/ft_out/public/ft_str_out.h
$(O_DIR)/_public/ft/ft_vprintf.h: libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/_public/ft/ft_wchar.h: libft/ft_base/public/ft_wchar.h
$(O_DIR)/_public/ft/getkey.h: libft/ft_getkey/public/getkey.h
$(O_DIR)/_public/ft/libft.h: libft/ft_base/public/libft.h
$(O_DIR)/_public/ft/range.h: srcs/range/public/range.h
$(O_DIR)/_public/ft/term.h: libft/ft_term/public/term.h