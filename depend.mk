INCLUDE_FLAGS += -I$(O_DIR)/_public
LINK_FLAGS += -ltermcap
MAINS += sh21
OBJ_DIR_TREE += $(O_DIR)/srcs/strset/ $(O_DIR)/srcs/spanlist/ \
	$(O_DIR)/srcs/sh_parser_context/ $(O_DIR)/srcs/sh_parser/ \
	$(O_DIR)/srcs/sh_exec/ $(O_DIR)/srcs/sh_default_builtins/builtins/ \
	$(O_DIR)/srcs/sh_default_builtins/ $(O_DIR)/srcs/sh_context/ \
	$(O_DIR)/srcs/sh_ast/ $(O_DIR)/srcs/main/ $(O_DIR)/srcs/ft_utils/ \
	$(O_DIR)/srcs/ft_str_list/ $(O_DIR)/srcs/editor_term/ \
	$(O_DIR)/srcs/editor_bindings/default/ $(O_DIR)/srcs/editor_bindings/ \
	$(O_DIR)/srcs/editor/ $(O_DIR)/srcs/ $(O_DIR)/libft/ft_vector/ \
	$(O_DIR)/libft/ft_tokenizer/ $(O_DIR)/libft/ft_term/ \
	$(O_DIR)/libft/ft_set/ $(O_DIR)/libft/ft_printf/formats/ \
	$(O_DIR)/libft/ft_printf/ $(O_DIR)/libft/ft_out/ $(O_DIR)/libft/ft_list/ \
	$(O_DIR)/libft/ft_lexer/ $(O_DIR)/libft/ft_in/ $(O_DIR)/libft/ft_getkey/ \
	$(O_DIR)/libft/ft_file_in/ $(O_DIR)/libft/ft_file/ $(O_DIR)/libft/ft_dstr/ \
	$(O_DIR)/libft/ft_buff_in/ $(O_DIR)/libft/ft_base/ $(O_DIR)/libft/ft_argv/ \
	$(O_DIR)/libft/ $(O_DIR)/_public/sh/ $(O_DIR)/_public/ft/ \
	$(O_DIR)/_public/ $(O_DIR)/
O_FILES += $(O_DIR)/srcs/editor/editor_cursors.o \
	$(O_DIR)/srcs/editor/editor_put.o $(O_DIR)/srcs/editor/editor_write.o \
	$(O_DIR)/srcs/editor/utils.o \
	$(O_DIR)/srcs/editor_bindings/default/editor_move_bind.o \
	$(O_DIR)/srcs/editor_bindings/default/editor_move_bind_word.o \
	$(O_DIR)/srcs/editor_bindings/editor_bindings.o \
	$(O_DIR)/srcs/editor_term/init.o $(O_DIR)/srcs/editor_term/render.o \
	$(O_DIR)/srcs/editor_term/update.o $(O_DIR)/libft/ft_argv/arg.o \
	$(O_DIR)/libft/ft_argv/argv_argv.o $(O_DIR)/libft/ft_argv/argv_argv_t.o \
	$(O_DIR)/libft/ft_argv/opt.o $(O_DIR)/libft/ft_base/ft_abs.o \
	$(O_DIR)/libft/ft_base/ft_assert.o $(O_DIR)/libft/ft_base/ft_atoib.o \
	$(O_DIR)/libft/ft_base/ft_bitset.o $(O_DIR)/libft/ft_base/ft_clock.o \
	$(O_DIR)/libft/ft_base/ft_emalloc.o $(O_DIR)/libft/ft_base/ft_escape.o \
	$(O_DIR)/libft/ft_base/ft_getenv.o $(O_DIR)/libft/ft_base/ft_is.o \
	$(O_DIR)/libft/ft_base/ft_max.o $(O_DIR)/libft/ft_base/ft_memdup.o \
	$(O_DIR)/libft/ft_base/ft_memfill.o $(O_DIR)/libft/ft_base/ft_memrcpy.o \
	$(O_DIR)/libft/ft_base/ft_memstart.o $(O_DIR)/libft/ft_base/ft_memswap.o \
	$(O_DIR)/libft/ft_base/ft_min.o $(O_DIR)/libft/ft_base/ft_rand.o \
	$(O_DIR)/libft/ft_base/ft_sort.o $(O_DIR)/libft/ft_base/ft_strchri.o \
	$(O_DIR)/libft/ft_base/ft_strlen.o $(O_DIR)/libft/ft_base/ft_sub.o \
	$(O_DIR)/libft/ft_base/ft_subfind.o $(O_DIR)/libft/ft_base/ft_subnext.o \
	$(O_DIR)/libft/ft_base/ft_subto.o $(O_DIR)/libft/ft_base/ft_unescape.o \
	$(O_DIR)/libft/ft_base/ft_widetoa.o $(O_DIR)/libft/ft_base/ft_wstrconv.o \
	$(O_DIR)/libft/ft_base/ft_wstrlen.o $(O_DIR)/libft/ft_base/ft_wstrnconv.o \
	$(O_DIR)/libft/ft_buff_in/ft_buff_in.o $(O_DIR)/libft/ft_dstr/ft_dstradd.o \
	$(O_DIR)/libft/ft_dstr/ft_dstrclear.o \
	$(O_DIR)/libft/ft_dstr/ft_dstrextend.o \
	$(O_DIR)/libft/ft_dstr/ft_dstrspan.o $(O_DIR)/libft/ft_file/open.o \
	$(O_DIR)/libft/ft_file_in/file_in.o \
	$(O_DIR)/libft/ft_file_in/file_in_open.o \
	$(O_DIR)/libft/ft_getkey/ft_getkey.o $(O_DIR)/libft/ft_getkey/get_seq.o \
	$(O_DIR)/libft/ft_getkey/next_char.o $(O_DIR)/libft/ft_in/ft_in_refresh.o \
	$(O_DIR)/libft/ft_in/ft_read.o $(O_DIR)/libft/ft_in/ft_readto.o \
	$(O_DIR)/libft/ft_lexer/build.o $(O_DIR)/libft/ft_lexer/lexer.o \
	$(O_DIR)/libft/ft_list/ft_listadd.o $(O_DIR)/libft/ft_list/ft_listremove.o \
	$(O_DIR)/libft/ft_list/ft_listsort.o $(O_DIR)/libft/ft_out/ft_putchar.o \
	$(O_DIR)/libft/ft_out/ft_putendl.o $(O_DIR)/libft/ft_out/ft_putfloat.o \
	$(O_DIR)/libft/ft_out/ft_putint.o $(O_DIR)/libft/ft_out/ft_putnchar.o \
	$(O_DIR)/libft/ft_out/ft_putpad.o $(O_DIR)/libft/ft_out/ft_putsub.o \
	$(O_DIR)/libft/ft_out/ft_putuint.o $(O_DIR)/libft/ft_out/ft_str_out.o \
	$(O_DIR)/libft/ft_out/ft_write.o $(O_DIR)/libft/ft_out/ft_write_char.o \
	$(O_DIR)/libft/ft_out/ft_write_nchar.o $(O_DIR)/libft/ft_out/out_formats.o \
	$(O_DIR)/libft/ft_printf/args_utils.o \
	$(O_DIR)/libft/ft_printf/exec_format.o \
	$(O_DIR)/libft/ft_printf/exec_subformat.o \
	$(O_DIR)/libft/ft_printf/formats/char.o \
	$(O_DIR)/libft/ft_printf/formats/int.o \
	$(O_DIR)/libft/ft_printf/formats/misc.o \
	$(O_DIR)/libft/ft_printf/formats/str.o \
	$(O_DIR)/libft/ft_printf/ft_aprintf.o $(O_DIR)/libft/ft_printf/ft_logf.o \
	$(O_DIR)/libft/ft_printf/ft_out.o $(O_DIR)/libft/ft_printf/ft_printf.o \
	$(O_DIR)/libft/ft_printf/ft_putf.o $(O_DIR)/libft/ft_printf/ft_vprintf.o \
	$(O_DIR)/libft/ft_set/begin.o $(O_DIR)/libft/ft_set/get.o \
	$(O_DIR)/libft/ft_set/insert.o $(O_DIR)/libft/ft_set/iter.o \
	$(O_DIR)/libft/ft_set/remove.o $(O_DIR)/libft/ft_set/utils.o \
	$(O_DIR)/srcs/spanlist/ft_spanlist_clear.o \
	$(O_DIR)/srcs/spanlist/ft_spanlist_iter.o \
	$(O_DIR)/srcs/spanlist/ft_spanlist_push.o \
	$(O_DIR)/srcs/spanlist/ft_spanlist_set.o \
	$(O_DIR)/srcs/ft_str_list/ft_str_list.o $(O_DIR)/srcs/strset/strset.o \
	$(O_DIR)/libft/ft_term/ft_tcursor.o $(O_DIR)/libft/ft_term/ft_tinit.o \
	$(O_DIR)/libft/ft_term/ft_tput.o $(O_DIR)/libft/ft_term/ft_trestore.o \
	$(O_DIR)/libft/ft_term/term_out.o $(O_DIR)/libft/ft_term/utils.o \
	$(O_DIR)/libft/ft_tokenizer/ft_tokenize.o \
	$(O_DIR)/libft/ft_tokenizer/ft_tokenmap_builder.o \
	$(O_DIR)/srcs/ft_utils/ft_subis.o $(O_DIR)/libft/ft_vector/ft_vclear.o \
	$(O_DIR)/libft/ft_vector/ft_vpush.o $(O_DIR)/libft/ft_vector/ft_vreserve.o \
	$(O_DIR)/libft/ft_vector/ft_vspan.o $(O_DIR)/srcs/main/main.o \
	$(O_DIR)/srcs/sh_ast/copy_compound.o $(O_DIR)/srcs/sh_ast/destroy_cmd.o \
	$(O_DIR)/srcs/sh_ast/destroy_compound.o \
	$(O_DIR)/srcs/sh_ast/destroy_text.o \
	$(O_DIR)/srcs/sh_context/context_init.o \
	$(O_DIR)/srcs/sh_context/sh_builtin.o $(O_DIR)/srcs/sh_context/sh_env.o \
	$(O_DIR)/srcs/sh_context/sh_function.o $(O_DIR)/srcs/sh_context/sh_path.o \
	$(O_DIR)/srcs/sh_context/sh_var.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/alias.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/cd.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/echo.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/exit.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/false.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/hash.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/pwd.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/set.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/shift.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/source.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/true.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/unalias.o \
	$(O_DIR)/srcs/sh_default_builtins/builtins/unset.o \
	$(O_DIR)/srcs/sh_default_builtins/sh_init_default_builtins.o \
	$(O_DIR)/srcs/sh_exec/sh_exec_cmd.o \
	$(O_DIR)/srcs/sh_exec/sh_exec_cmd_for_clause.o \
	$(O_DIR)/srcs/sh_exec/sh_exec_cmd_if_clause.o \
	$(O_DIR)/srcs/sh_exec/sh_exec_cmd_simple.o \
	$(O_DIR)/srcs/sh_exec/sh_exec_cmd_time_clause.o \
	$(O_DIR)/srcs/sh_exec/sh_exec_cmd_while_clause.o \
	$(O_DIR)/srcs/sh_exec/sh_exec_compound.o \
	$(O_DIR)/srcs/sh_exec/sh_exec_redir.o $(O_DIR)/srcs/sh_exec/sh_exec_text.o \
	$(O_DIR)/srcs/sh_exec/utils.o $(O_DIR)/srcs/sh_parser/sh_parse_cmd.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_compound.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_do_clause.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_for_clause.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_if_clause.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_line.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_redir.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_text.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_text_comment.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_text_escape_sequence.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_text_string.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_text_subshell.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_text_subst_param.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_utils.o \
	$(O_DIR)/srcs/sh_parser/sh_parse_while_clause.o \
	$(O_DIR)/srcs/sh_parser_context/alias.o
PUBLIC_LINKS += $(O_DIR)/_public/editor.h $(O_DIR)/_public/editor_bindings.h \
	$(O_DIR)/_public/editor_default_bindings.h $(O_DIR)/_public/editor_term.h \
	$(O_DIR)/_public/ft/argv.h $(O_DIR)/_public/ft/buff_in.h \
	$(O_DIR)/_public/ft/file.h $(O_DIR)/_public/ft/file_in.h \
	$(O_DIR)/_public/ft/ft_colors.h $(O_DIR)/_public/ft/ft_dstr.h \
	$(O_DIR)/_public/ft/ft_in.h $(O_DIR)/_public/ft/ft_list.h \
	$(O_DIR)/_public/ft/ft_out.h $(O_DIR)/_public/ft/ft_printf.h \
	$(O_DIR)/_public/ft/ft_str_out.h $(O_DIR)/_public/ft/ft_vector.h \
	$(O_DIR)/_public/ft/ft_vprintf.h $(O_DIR)/_public/ft/ft_wchar.h \
	$(O_DIR)/_public/ft/getkey.h $(O_DIR)/_public/ft/lexer.h \
	$(O_DIR)/_public/ft/libft.h $(O_DIR)/_public/ft/set.h \
	$(O_DIR)/_public/ft/spanlist.h $(O_DIR)/_public/ft/str_list.h \
	$(O_DIR)/_public/ft/strset.h $(O_DIR)/_public/ft/term.h \
	$(O_DIR)/_public/ft/tokenizer.h $(O_DIR)/_public/ft/tokenmap_builder.h \
	$(O_DIR)/_public/ft/utils.h $(O_DIR)/_public/sh/ast.h \
	$(O_DIR)/_public/sh/context.h $(O_DIR)/_public/sh/default_builtins.h \
	$(O_DIR)/_public/sh/exec.h $(O_DIR)/_public/sh/parser.h \
	$(O_DIR)/_public/sh/parser_context.h

# module editor
$(O_DIR)/srcs/editor/editor_cursors.o: srcs/editor/editor_cursors.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_vector/public/ft_vector.h \
	srcs/editor/public/editor.h
$(O_DIR)/srcs/editor/editor_put.o: srcs/editor/editor_put.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_vector/public/ft_vector.h \
	srcs/editor/public/editor.h
$(O_DIR)/srcs/editor/editor_write.o: srcs/editor/editor_write.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_vector/public/ft_vector.h \
	srcs/editor/public/editor.h
$(O_DIR)/srcs/editor/utils.o: srcs/editor/utils.c libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_vector/public/ft_vector.h srcs/editor/public/editor.h

# module editor_bindings
$(O_DIR)/srcs/editor_bindings/default/editor_move_bind.o: \
	srcs/editor_bindings/default/editor_move_bind.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_getkey/public/getkey.h libft/ft_in/public/ft_in.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/editor/public/editor.h srcs/editor_bindings/public/editor_bindings.h \
	srcs/editor_bindings/public/editor_default_bindings.h
$(O_DIR)/srcs/editor_bindings/default/editor_move_bind_word.o: \
	srcs/editor_bindings/default/editor_move_bind_word.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_getkey/public/getkey.h libft/ft_in/public/ft_in.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/editor/public/editor.h srcs/editor_bindings/public/editor_bindings.h \
	srcs/editor_bindings/public/editor_default_bindings.h
$(O_DIR)/srcs/editor_bindings/editor_bindings.o: \
	srcs/editor_bindings/editor_bindings.c libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_getkey/public/getkey.h \
	libft/ft_in/public/ft_in.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/editor/public/editor.h \
	srcs/editor_bindings/public/editor_bindings.h

# module editor_term
$(O_DIR)/srcs/editor_term/init.o: srcs/editor_term/init.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_vector/public/ft_vector.h \
	srcs/editor/public/editor.h srcs/editor_term/p_editor_term.h \
	srcs/editor_term/public/editor_term.h
$(O_DIR)/srcs/editor_term/render.o: srcs/editor_term/render.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_vector/public/ft_vector.h \
	srcs/editor/public/editor.h srcs/editor_term/p_editor_term.h \
	srcs/editor_term/public/editor_term.h
$(O_DIR)/srcs/editor_term/update.o: srcs/editor_term/update.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_vector/public/ft_vector.h \
	srcs/editor/public/editor.h srcs/editor_term/p_editor_term.h \
	srcs/editor_term/public/editor_term.h

$(O_DIR)/srcs/editor_term/init.o $(O_DIR)/srcs/editor_term/render.o \
$(O_DIR)/srcs/editor_term/update.o: INCLUDE_FLAGS += -Isrcs/editor_term

# module ft::argv
$(O_DIR)/libft/ft_argv/arg.o: libft/ft_argv/arg.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_argv/argv_argv.o: libft/ft_argv/argv_argv.c \
	libft/ft_argv/p_argv.h libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_argv/argv_argv_t.o: libft/ft_argv/argv_argv_t.c \
	libft/ft_argv/p_argv.h libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_argv/opt.o: libft/ft_argv/opt.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h

$(O_DIR)/libft/ft_argv/arg.o $(O_DIR)/libft/ft_argv/argv_argv.o \
$(O_DIR)/libft/ft_argv/argv_argv_t.o $(O_DIR)/libft/ft_argv/opt.o: \
	INCLUDE_FLAGS += -Ilibft/ft_argv

# module ft::base
$(O_DIR)/libft/ft_base/ft_abs.o: libft/ft_base/ft_abs.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_assert.o: libft/ft_base/ft_assert.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_atoib.o: libft/ft_base/ft_atoib.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_bitset.o: libft/ft_base/ft_bitset.c \
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
$(O_DIR)/libft/ft_base/ft_memdup.o: libft/ft_base/ft_memdup.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memfill.o: libft/ft_base/ft_memfill.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memrcpy.o: libft/ft_base/ft_memrcpy.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memstart.o: libft/ft_base/ft_memstart.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_memswap.o: libft/ft_base/ft_memswap.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_min.o: libft/ft_base/ft_min.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_rand.o: libft/ft_base/ft_rand.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_sort.o: libft/ft_base/ft_sort.c \
	libft/ft_base/p_sort.h libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_strchri.o: libft/ft_base/ft_strchri.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_strlen.o: libft/ft_base/ft_strlen.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_sub.o: libft/ft_base/ft_sub.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_subfind.o: libft/ft_base/ft_subfind.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_subnext.o: libft/ft_base/ft_subnext.c \
	libft/ft_base/public/libft.h
$(O_DIR)/libft/ft_base/ft_subto.o: libft/ft_base/ft_subto.c \
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

$(O_DIR)/libft/ft_base/ft_abs.o $(O_DIR)/libft/ft_base/ft_assert.o \
$(O_DIR)/libft/ft_base/ft_atoib.o $(O_DIR)/libft/ft_base/ft_bitset.o \
$(O_DIR)/libft/ft_base/ft_clock.o $(O_DIR)/libft/ft_base/ft_emalloc.o \
$(O_DIR)/libft/ft_base/ft_escape.o $(O_DIR)/libft/ft_base/ft_getenv.o \
$(O_DIR)/libft/ft_base/ft_is.o $(O_DIR)/libft/ft_base/ft_max.o \
$(O_DIR)/libft/ft_base/ft_memdup.o $(O_DIR)/libft/ft_base/ft_memfill.o \
$(O_DIR)/libft/ft_base/ft_memrcpy.o $(O_DIR)/libft/ft_base/ft_memstart.o \
$(O_DIR)/libft/ft_base/ft_memswap.o $(O_DIR)/libft/ft_base/ft_min.o \
$(O_DIR)/libft/ft_base/ft_rand.o $(O_DIR)/libft/ft_base/ft_sort.o \
$(O_DIR)/libft/ft_base/ft_strchri.o $(O_DIR)/libft/ft_base/ft_strlen.o \
$(O_DIR)/libft/ft_base/ft_sub.o $(O_DIR)/libft/ft_base/ft_subfind.o \
$(O_DIR)/libft/ft_base/ft_subnext.o $(O_DIR)/libft/ft_base/ft_subto.o \
$(O_DIR)/libft/ft_base/ft_unescape.o $(O_DIR)/libft/ft_base/ft_widetoa.o \
$(O_DIR)/libft/ft_base/ft_wstrconv.o $(O_DIR)/libft/ft_base/ft_wstrlen.o \
$(O_DIR)/libft/ft_base/ft_wstrnconv.o: INCLUDE_FLAGS += -Ilibft/ft_base

# module ft::buff_in
$(O_DIR)/libft/ft_buff_in/ft_buff_in.o: libft/ft_buff_in/ft_buff_in.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h

# module ft::dstr
$(O_DIR)/libft/ft_dstr/ft_dstradd.o: libft/ft_dstr/ft_dstradd.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h
$(O_DIR)/libft/ft_dstr/ft_dstrclear.o: libft/ft_dstr/ft_dstrclear.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h
$(O_DIR)/libft/ft_dstr/ft_dstrextend.o: libft/ft_dstr/ft_dstrextend.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h
$(O_DIR)/libft/ft_dstr/ft_dstrspan.o: libft/ft_dstr/ft_dstrspan.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h

# module ft::file
$(O_DIR)/libft/ft_file/open.o: libft/ft_file/open.c \
	libft/ft_base/public/libft.h libft/ft_file/public/file.h

# module ft::file_in
$(O_DIR)/libft/ft_file_in/file_in.o: libft/ft_file_in/file_in.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_file_in/public/file_in.h libft/ft_in/public/ft_in.h
$(O_DIR)/libft/ft_file_in/file_in_open.o: libft/ft_file_in/file_in_open.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_file/public/file.h libft/ft_file_in/public/file_in.h \
	libft/ft_in/public/ft_in.h

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

# module ft::in
$(O_DIR)/libft/ft_in/ft_in_refresh.o: libft/ft_in/ft_in_refresh.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h
$(O_DIR)/libft/ft_in/ft_read.o: libft/ft_in/ft_read.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h
$(O_DIR)/libft/ft_in/ft_readto.o: libft/ft_in/ft_readto.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h

# module ft::lexer
$(O_DIR)/libft/ft_lexer/build.o: libft/ft_lexer/build.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_lexer/p_lexer.h libft/ft_lexer/public/lexer.h \
	libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_tokenizer/public/tokenmap_builder.h \
	libft/ft_vector/public/ft_vector.h
$(O_DIR)/libft/ft_lexer/lexer.o: libft/ft_lexer/lexer.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_lexer/p_lexer.h libft/ft_lexer/public/lexer.h \
	libft/ft_tokenizer/public/tokenizer.h libft/ft_vector/public/ft_vector.h

$(O_DIR)/libft/ft_lexer/build.o $(O_DIR)/libft/ft_lexer/lexer.o: INCLUDE_FLAGS \
	+= -Ilibft/ft_lexer

# module ft::list
$(O_DIR)/libft/ft_list/ft_listadd.o: libft/ft_list/ft_listadd.c \
	libft/ft_base/public/libft.h libft/ft_list/public/ft_list.h
$(O_DIR)/libft/ft_list/ft_listremove.o: libft/ft_list/ft_listremove.c \
	libft/ft_base/public/libft.h libft/ft_list/public/ft_list.h
$(O_DIR)/libft/ft_list/ft_listsort.o: libft/ft_list/ft_listsort.c \
	libft/ft_base/public/libft.h libft/ft_list/public/ft_list.h

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
$(O_DIR)/libft/ft_out/ft_putsub.o $(O_DIR)/libft/ft_out/ft_putuint.o \
$(O_DIR)/libft/ft_out/ft_str_out.o $(O_DIR)/libft/ft_out/ft_write.o \
$(O_DIR)/libft/ft_out/ft_write_char.o $(O_DIR)/libft/ft_out/ft_write_nchar.o \
$(O_DIR)/libft/ft_out/out_formats.o: INCLUDE_FLAGS += -Ilibft/ft_out

# module ft::printf
$(O_DIR)/libft/ft_printf/args_utils.o: libft/ft_printf/args_utils.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/exec_format.o: libft/ft_printf/exec_format.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/exec_subformat.o: libft/ft_printf/exec_subformat.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_out/public/ft_str_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/formats/char.o: libft/ft_printf/formats/char.c \
	libft/ft_base/public/ft_wchar.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/formats/int.o: libft/ft_printf/formats/int.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/formats/misc.o: libft/ft_printf/formats/misc.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/formats/str.o: libft/ft_printf/formats/str.c \
	libft/ft_base/public/ft_wchar.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_aprintf.o: libft/ft_printf/ft_aprintf.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_logf.o: libft/ft_printf/ft_logf.c \
	libft/ft_base/public/ft_colors.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/printf_internal.h libft/ft_printf/public/ft_printf.h \
	libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_out.o: libft/ft_printf/ft_out.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_printf.o: libft/ft_printf/ft_printf.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_putf.o: libft/ft_printf/ft_putf.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/libft/ft_printf/ft_vprintf.o: libft/ft_printf/ft_vprintf.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/printf_internal.h \
	libft/ft_printf/public/ft_printf.h libft/ft_printf/public/ft_vprintf.h

$(O_DIR)/libft/ft_printf/args_utils.o $(O_DIR)/libft/ft_printf/exec_format.o \
$(O_DIR)/libft/ft_printf/exec_subformat.o \
$(O_DIR)/libft/ft_printf/formats/char.o $(O_DIR)/libft/ft_printf/formats/int.o \
$(O_DIR)/libft/ft_printf/formats/misc.o $(O_DIR)/libft/ft_printf/formats/str.o \
$(O_DIR)/libft/ft_printf/ft_aprintf.o $(O_DIR)/libft/ft_printf/ft_logf.o \
$(O_DIR)/libft/ft_printf/ft_out.o $(O_DIR)/libft/ft_printf/ft_printf.o \
$(O_DIR)/libft/ft_printf/ft_putf.o $(O_DIR)/libft/ft_printf/ft_vprintf.o: \
	INCLUDE_FLAGS += -Ilibft/ft_printf

# module ft::set
$(O_DIR)/libft/ft_set/begin.o: libft/ft_set/begin.c \
	libft/ft_base/public/libft.h libft/ft_set/internal.h \
	libft/ft_set/public/set.h
$(O_DIR)/libft/ft_set/get.o: libft/ft_set/get.c libft/ft_base/public/libft.h \
	libft/ft_set/internal.h libft/ft_set/public/set.h
$(O_DIR)/libft/ft_set/insert.o: libft/ft_set/insert.c \
	libft/ft_base/public/libft.h libft/ft_set/internal.h \
	libft/ft_set/public/set.h
$(O_DIR)/libft/ft_set/iter.o: libft/ft_set/iter.c libft/ft_base/public/libft.h \
	libft/ft_set/internal.h libft/ft_set/public/set.h
$(O_DIR)/libft/ft_set/remove.o: libft/ft_set/remove.c \
	libft/ft_base/public/libft.h libft/ft_set/internal.h \
	libft/ft_set/public/set.h
$(O_DIR)/libft/ft_set/utils.o: libft/ft_set/utils.c \
	libft/ft_base/public/libft.h libft/ft_set/internal.h \
	libft/ft_set/public/set.h

$(O_DIR)/libft/ft_set/begin.o $(O_DIR)/libft/ft_set/get.o \
$(O_DIR)/libft/ft_set/insert.o $(O_DIR)/libft/ft_set/iter.o \
$(O_DIR)/libft/ft_set/remove.o $(O_DIR)/libft/ft_set/utils.o: INCLUDE_FLAGS += \
	-Ilibft/ft_set

# module ft::spanlist
$(O_DIR)/srcs/spanlist/ft_spanlist_clear.o: srcs/spanlist/ft_spanlist_clear.c \
	libft/ft_base/public/libft.h libft/ft_list/public/ft_list.h \
	srcs/spanlist/public/spanlist.h
$(O_DIR)/srcs/spanlist/ft_spanlist_iter.o: srcs/spanlist/ft_spanlist_iter.c \
	libft/ft_base/public/libft.h libft/ft_list/public/ft_list.h \
	srcs/spanlist/public/spanlist.h
$(O_DIR)/srcs/spanlist/ft_spanlist_push.o: srcs/spanlist/ft_spanlist_push.c \
	libft/ft_base/public/libft.h libft/ft_list/public/ft_list.h \
	srcs/spanlist/public/spanlist.h
$(O_DIR)/srcs/spanlist/ft_spanlist_set.o: srcs/spanlist/ft_spanlist_set.c \
	libft/ft_base/public/libft.h libft/ft_list/public/ft_list.h \
	srcs/spanlist/public/spanlist.h

# module ft::str_list
$(O_DIR)/srcs/ft_str_list/ft_str_list.o: srcs/ft_str_list/ft_str_list.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	srcs/ft_str_list/public/str_list.h

# module ft::strset
$(O_DIR)/srcs/strset/strset.o: srcs/strset/strset.c \
	libft/ft_base/public/libft.h libft/ft_set/public/set.h \
	srcs/strset/public/strset.h

# module ft::term
$(O_DIR)/libft/ft_term/ft_tcursor.o: libft/ft_term/ft_tcursor.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_term/term_internal.h
$(O_DIR)/libft/ft_term/ft_tinit.o: libft/ft_term/ft_tinit.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_term/public/term.h \
	libft/ft_term/term_internal.h
$(O_DIR)/libft/ft_term/ft_tput.o: libft/ft_term/ft_tput.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_term/term_internal.h
$(O_DIR)/libft/ft_term/ft_trestore.o: libft/ft_term/ft_trestore.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h
$(O_DIR)/libft/ft_term/term_out.o: libft/ft_term/term_out.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_term/term_internal.h
$(O_DIR)/libft/ft_term/utils.o: libft/ft_term/utils.c \
	libft/ft_base/public/libft.h libft/ft_out/public/ft_out.h \
	libft/ft_term/public/term.h libft/ft_term/term_internal.h

$(O_DIR)/libft/ft_term/ft_tcursor.o $(O_DIR)/libft/ft_term/ft_tinit.o \
$(O_DIR)/libft/ft_term/ft_tput.o $(O_DIR)/libft/ft_term/ft_trestore.o \
$(O_DIR)/libft/ft_term/term_out.o $(O_DIR)/libft/ft_term/utils.o: \
	INCLUDE_FLAGS += -Ilibft/ft_term

# module ft::tokenizer
$(O_DIR)/libft/ft_tokenizer/ft_tokenize.o: libft/ft_tokenizer/ft_tokenize.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_tokenizer/public/tokenizer.h
$(O_DIR)/libft/ft_tokenizer/ft_tokenmap_builder.o: \
	libft/ft_tokenizer/ft_tokenmap_builder.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_tokenizer/public/tokenmap_builder.h \
	libft/ft_vector/public/ft_vector.h

$(O_DIR)/libft/ft_tokenizer/ft_tokenize.o \
$(O_DIR)/libft/ft_tokenizer/ft_tokenmap_builder.o: INCLUDE_FLAGS += \
	-Ilibft/ft_tokenizer

# module ft::utils
$(O_DIR)/srcs/ft_utils/ft_subis.o: srcs/ft_utils/ft_subis.c \
	libft/ft_base/public/libft.h srcs/ft_utils/public/utils.h

# module ft::vector
$(O_DIR)/libft/ft_vector/ft_vclear.o: libft/ft_vector/ft_vclear.c \
	libft/ft_base/public/libft.h libft/ft_vector/public/ft_vector.h
$(O_DIR)/libft/ft_vector/ft_vpush.o: libft/ft_vector/ft_vpush.c \
	libft/ft_base/public/libft.h libft/ft_vector/public/ft_vector.h
$(O_DIR)/libft/ft_vector/ft_vreserve.o: libft/ft_vector/ft_vreserve.c \
	libft/ft_base/public/libft.h libft/ft_vector/public/ft_vector.h
$(O_DIR)/libft/ft_vector/ft_vspan.o: libft/ft_vector/ft_vspan.c \
	libft/ft_base/public/libft.h libft/ft_vector/public/ft_vector.h

# module sh21
$(O_DIR)/srcs/main/main.o: srcs/main/main.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/ft_colors.h libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_file_in/public/file_in.h libft/ft_getkey/public/getkey.h \
	libft/ft_in/public/ft_in.h libft/ft_list/public/ft_list.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_term/public/term.h \
	libft/ft_tokenizer/public/tokenizer.h libft/ft_vector/public/ft_vector.h \
	srcs/editor/public/editor.h srcs/editor_bindings/public/editor_bindings.h \
	srcs/editor_bindings/public/editor_default_bindings.h \
	srcs/editor_term/public/editor_term.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_exec/public/exec.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h \
	srcs/spanlist/public/spanlist.h srcs/strset/public/strset.h

$(O_DIR)/srcs/main/main.o: INCLUDE_FLAGS += -Isrcs/main

# module sh::ast
$(O_DIR)/srcs/sh_ast/copy_compound.o: srcs/sh_ast/copy_compound.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_utils/public/utils.h \
	srcs/sh_ast/public/ast.h
$(O_DIR)/srcs/sh_ast/destroy_cmd.o: srcs/sh_ast/destroy_cmd.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/p_destroy_cmd.h \
	srcs/sh_ast/public/ast.h
$(O_DIR)/srcs/sh_ast/destroy_compound.o: srcs/sh_ast/destroy_compound.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/p_destroy_cmd.h \
	srcs/sh_ast/public/ast.h
$(O_DIR)/srcs/sh_ast/destroy_text.o: srcs/sh_ast/destroy_text.c \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/p_destroy_cmd.h \
	srcs/sh_ast/public/ast.h

$(O_DIR)/srcs/sh_ast/copy_compound.o $(O_DIR)/srcs/sh_ast/destroy_cmd.o \
$(O_DIR)/srcs/sh_ast/destroy_compound.o $(O_DIR)/srcs/sh_ast/destroy_text.o: \
	INCLUDE_FLAGS += -Isrcs/sh_ast

# module sh::context
$(O_DIR)/srcs/sh_context/context_init.o: srcs/sh_context/context_init.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/p_sh_context.h \
	srcs/sh_context/public/context.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_context/sh_builtin.o: srcs/sh_context/sh_builtin.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/p_sh_context.h \
	srcs/sh_context/public/context.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_context/sh_env.o: srcs/sh_context/sh_env.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/p_sh_context.h \
	srcs/sh_context/public/context.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_context/sh_function.o: srcs/sh_context/sh_function.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/ft_utils/public/utils.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/p_sh_context.h srcs/sh_context/public/context.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_context/sh_path.o: srcs/sh_context/sh_path.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/ft_utils/public/utils.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/p_sh_context.h srcs/sh_context/public/context.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_context/sh_var.o: srcs/sh_context/sh_var.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/p_sh_context.h \
	srcs/sh_context/public/context.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h

$(O_DIR)/srcs/sh_context/context_init.o $(O_DIR)/srcs/sh_context/sh_builtin.o \
$(O_DIR)/srcs/sh_context/sh_env.o $(O_DIR)/srcs/sh_context/sh_function.o \
$(O_DIR)/srcs/sh_context/sh_path.o $(O_DIR)/srcs/sh_context/sh_var.o: \
	INCLUDE_FLAGS += -Isrcs/sh_context

# module sh::default_builtins
$(O_DIR)/srcs/sh_default_builtins/builtins/alias.o: \
	srcs/sh_default_builtins/builtins/alias.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/cd.o: \
	srcs/sh_default_builtins/builtins/cd.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/echo.o: \
	srcs/sh_default_builtins/builtins/echo.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/exit.o: \
	srcs/sh_default_builtins/builtins/exit.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/false.o: \
	srcs/sh_default_builtins/builtins/false.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/hash.o: \
	srcs/sh_default_builtins/builtins/hash.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/pwd.o: \
	srcs/sh_default_builtins/builtins/pwd.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/set.o: \
	srcs/sh_default_builtins/builtins/set.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/shift.o: \
	srcs/sh_default_builtins/builtins/shift.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/source.o: \
	srcs/sh_default_builtins/builtins/source.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_file_in/public/file_in.h libft/ft_in/public/ft_in.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_exec/public/exec.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/true.o: \
	srcs/sh_default_builtins/builtins/true.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/unalias.o: \
	srcs/sh_default_builtins/builtins/unalias.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/builtins/unset.o: \
	srcs/sh_default_builtins/builtins/unset.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/ft_utils/public/utils.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_default_builtins/sh_init_default_builtins.o: \
	srcs/sh_default_builtins/sh_init_default_builtins.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/public/context.h \
	srcs/sh_default_builtins/p_default_builtins.h \
	srcs/sh_default_builtins/public/default_builtins.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h

$(O_DIR)/srcs/sh_default_builtins/builtins/alias.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/cd.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/echo.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/exit.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/false.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/hash.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/pwd.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/set.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/shift.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/source.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/true.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/unalias.o \
$(O_DIR)/srcs/sh_default_builtins/builtins/unset.o \
$(O_DIR)/srcs/sh_default_builtins/sh_init_default_builtins.o: INCLUDE_FLAGS += \
	-Isrcs/sh_default_builtins

# module sh::exec
$(O_DIR)/srcs/sh_exec/sh_exec_cmd.o: srcs/sh_exec/sh_exec_cmd.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/public/context.h \
	srcs/sh_exec/p_sh_exec.h srcs/sh_exec/public/exec.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_for_clause.o: \
	srcs/sh_exec/sh_exec_cmd_for_clause.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h srcs/sh_exec/p_sh_exec.h \
	srcs/sh_exec/public/exec.h srcs/sh_parser_context/public/parser_context.h \
	srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_if_clause.o: \
	srcs/sh_exec/sh_exec_cmd_if_clause.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h srcs/sh_exec/p_sh_exec.h \
	srcs/sh_exec/public/exec.h srcs/sh_parser_context/public/parser_context.h \
	srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_simple.o: srcs/sh_exec/sh_exec_cmd_simple.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/public/ft_printf.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/public/context.h \
	srcs/sh_exec/p_sh_exec.h srcs/sh_exec/public/exec.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_time_clause.o: \
	srcs/sh_exec/sh_exec_cmd_time_clause.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_out/public/ft_out.h libft/ft_printf/public/ft_printf.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h srcs/sh_exec/p_sh_exec.h \
	srcs/sh_exec/public/exec.h srcs/sh_parser_context/public/parser_context.h \
	srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_while_clause.o: \
	srcs/sh_exec/sh_exec_cmd_while_clause.c libft/ft_argv/public/argv.h \
	libft/ft_base/public/libft.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_set/public/set.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_str_list/public/str_list.h srcs/sh_ast/public/ast.h \
	srcs/sh_context/public/context.h srcs/sh_exec/p_sh_exec.h \
	srcs/sh_exec/public/exec.h srcs/sh_parser_context/public/parser_context.h \
	srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_exec/sh_exec_compound.o: srcs/sh_exec/sh_exec_compound.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/public/context.h \
	srcs/sh_exec/p_sh_exec.h srcs/sh_exec/public/exec.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_exec/sh_exec_redir.o: srcs/sh_exec/sh_exec_redir.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/public/ft_printf.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/public/context.h \
	srcs/sh_exec/p_sh_exec.h srcs/sh_exec/public/exec.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_exec/sh_exec_text.o: srcs/sh_exec/sh_exec_text.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/public/ft_printf.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/public/context.h \
	srcs/sh_exec/p_sh_exec.h srcs/sh_exec/public/exec.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h
$(O_DIR)/srcs/sh_exec/utils.o: srcs/sh_exec/utils.c \
	libft/ft_argv/public/argv.h libft/ft_base/public/libft.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_out/public/ft_out.h \
	libft/ft_printf/public/ft_printf.h libft/ft_set/public/set.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_str_list/public/str_list.h \
	srcs/sh_ast/public/ast.h srcs/sh_context/public/context.h \
	srcs/sh_exec/p_sh_exec.h srcs/sh_exec/public/exec.h \
	srcs/sh_parser_context/public/parser_context.h srcs/strset/public/strset.h

$(O_DIR)/srcs/sh_exec/sh_exec_cmd.o \
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_for_clause.o \
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_if_clause.o \
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_simple.o \
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_time_clause.o \
$(O_DIR)/srcs/sh_exec/sh_exec_cmd_while_clause.o \
$(O_DIR)/srcs/sh_exec/sh_exec_compound.o $(O_DIR)/srcs/sh_exec/sh_exec_redir.o \
$(O_DIR)/srcs/sh_exec/sh_exec_text.o $(O_DIR)/srcs/sh_exec/utils.o: \
	INCLUDE_FLAGS += -Isrcs/sh_exec

# module sh::parser
$(O_DIR)/srcs/sh_parser/sh_parse_cmd.o: srcs/sh_parser/sh_parse_cmd.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_lexer/public/lexer.h libft/ft_set/public/set.h \
	libft/ft_tokenizer/public/tokenizer.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_utils/public/utils.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_compound.o: \
	srcs/sh_parser/sh_parse_compound.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_lexer/public/lexer.h \
	libft/ft_set/public/set.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_do_clause.o: \
	srcs/sh_parser/sh_parse_do_clause.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_lexer/public/lexer.h \
	libft/ft_set/public/set.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_for_clause.o: \
	srcs/sh_parser/sh_parse_for_clause.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_lexer/public/lexer.h \
	libft/ft_set/public/set.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_vector/public/ft_vector.h srcs/ft_utils/public/utils.h \
	srcs/sh_ast/public/ast.h srcs/sh_parser/p_sh_parser.h \
	srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_if_clause.o: \
	srcs/sh_parser/sh_parse_if_clause.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_lexer/public/lexer.h \
	libft/ft_set/public/set.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_line.o: srcs/sh_parser/sh_parse_line.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_lexer/public/lexer.h libft/ft_set/public/set.h \
	libft/ft_tokenizer/public/tokenizer.h libft/ft_vector/public/ft_vector.h \
	srcs/sh_ast/public/ast.h srcs/sh_parser/p_sh_parser.h \
	srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_redir.o: srcs/sh_parser/sh_parse_redir.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_lexer/public/lexer.h libft/ft_set/public/set.h \
	libft/ft_tokenizer/public/tokenizer.h libft/ft_vector/public/ft_vector.h \
	srcs/sh_ast/public/ast.h srcs/sh_parser/p_sh_parser.h \
	srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_text.o: srcs/sh_parser/sh_parse_text.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_lexer/public/lexer.h libft/ft_set/public/set.h \
	libft/ft_tokenizer/public/tokenizer.h libft/ft_vector/public/ft_vector.h \
	srcs/sh_ast/public/ast.h srcs/sh_parser/p_sh_parser.h \
	srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_text_comment.o: \
	srcs/sh_parser/sh_parse_text_comment.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_lexer/public/lexer.h \
	libft/ft_set/public/set.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_text_escape_sequence.o: \
	srcs/sh_parser/sh_parse_text_escape_sequence.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_lexer/public/lexer.h libft/ft_set/public/set.h \
	libft/ft_tokenizer/public/tokenizer.h libft/ft_vector/public/ft_vector.h \
	srcs/sh_ast/public/ast.h srcs/sh_parser/p_sh_parser.h \
	srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_text_string.o: \
	srcs/sh_parser/sh_parse_text_string.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_lexer/public/lexer.h \
	libft/ft_set/public/set.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_text_subshell.o: \
	srcs/sh_parser/sh_parse_text_subshell.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_lexer/public/lexer.h \
	libft/ft_set/public/set.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_text_subst_param.o: \
	srcs/sh_parser/sh_parse_text_subst_param.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_lexer/public/lexer.h \
	libft/ft_set/public/set.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_utils.o: srcs/sh_parser/sh_parse_utils.c \
	libft/ft_base/public/libft.h libft/ft_buff_in/public/buff_in.h \
	libft/ft_dstr/public/ft_dstr.h libft/ft_in/public/ft_in.h \
	libft/ft_lexer/public/lexer.h libft/ft_set/public/set.h \
	libft/ft_tokenizer/public/tokenizer.h libft/ft_vector/public/ft_vector.h \
	srcs/ft_utils/public/utils.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h
$(O_DIR)/srcs/sh_parser/sh_parse_while_clause.o: \
	srcs/sh_parser/sh_parse_while_clause.c libft/ft_base/public/libft.h \
	libft/ft_buff_in/public/buff_in.h libft/ft_dstr/public/ft_dstr.h \
	libft/ft_in/public/ft_in.h libft/ft_lexer/public/lexer.h \
	libft/ft_set/public/set.h libft/ft_tokenizer/public/tokenizer.h \
	libft/ft_vector/public/ft_vector.h srcs/sh_ast/public/ast.h \
	srcs/sh_parser/p_sh_parser.h srcs/sh_parser/public/parser.h \
	srcs/sh_parser_context/public/parser_context.h

$(O_DIR)/srcs/sh_parser/sh_parse_cmd.o \
$(O_DIR)/srcs/sh_parser/sh_parse_compound.o \
$(O_DIR)/srcs/sh_parser/sh_parse_do_clause.o \
$(O_DIR)/srcs/sh_parser/sh_parse_for_clause.o \
$(O_DIR)/srcs/sh_parser/sh_parse_if_clause.o \
$(O_DIR)/srcs/sh_parser/sh_parse_line.o \
$(O_DIR)/srcs/sh_parser/sh_parse_redir.o \
$(O_DIR)/srcs/sh_parser/sh_parse_text.o \
$(O_DIR)/srcs/sh_parser/sh_parse_text_comment.o \
$(O_DIR)/srcs/sh_parser/sh_parse_text_escape_sequence.o \
$(O_DIR)/srcs/sh_parser/sh_parse_text_string.o \
$(O_DIR)/srcs/sh_parser/sh_parse_text_subshell.o \
$(O_DIR)/srcs/sh_parser/sh_parse_text_subst_param.o \
$(O_DIR)/srcs/sh_parser/sh_parse_utils.o \
$(O_DIR)/srcs/sh_parser/sh_parse_while_clause.o: INCLUDE_FLAGS += \
	-Isrcs/sh_parser

# module sh::parser_context
$(O_DIR)/srcs/sh_parser_context/alias.o: srcs/sh_parser_context/alias.c \
	libft/ft_base/public/libft.h libft/ft_set/public/set.h \
	srcs/sh_parser_context/public/parser_context.h

sh21: $(O_FILES)

# public links
$(O_DIR)/_public/editor.h: srcs/editor/public/editor.h
$(O_DIR)/_public/editor_bindings.h: \
srcs/editor_bindings/public/editor_bindings.h
$(O_DIR)/_public/editor_default_bindings.h: \
srcs/editor_bindings/public/editor_default_bindings.h
$(O_DIR)/_public/editor_term.h: srcs/editor_term/public/editor_term.h
$(O_DIR)/_public/ft/argv.h: libft/ft_argv/public/argv.h
$(O_DIR)/_public/ft/buff_in.h: libft/ft_buff_in/public/buff_in.h
$(O_DIR)/_public/ft/file.h: libft/ft_file/public/file.h
$(O_DIR)/_public/ft/file_in.h: libft/ft_file_in/public/file_in.h
$(O_DIR)/_public/ft/ft_colors.h: libft/ft_base/public/ft_colors.h
$(O_DIR)/_public/ft/ft_dstr.h: libft/ft_dstr/public/ft_dstr.h
$(O_DIR)/_public/ft/ft_in.h: libft/ft_in/public/ft_in.h
$(O_DIR)/_public/ft/ft_list.h: libft/ft_list/public/ft_list.h
$(O_DIR)/_public/ft/ft_out.h: libft/ft_out/public/ft_out.h
$(O_DIR)/_public/ft/ft_printf.h: libft/ft_printf/public/ft_printf.h
$(O_DIR)/_public/ft/ft_str_out.h: libft/ft_out/public/ft_str_out.h
$(O_DIR)/_public/ft/ft_vector.h: libft/ft_vector/public/ft_vector.h
$(O_DIR)/_public/ft/ft_vprintf.h: libft/ft_printf/public/ft_vprintf.h
$(O_DIR)/_public/ft/ft_wchar.h: libft/ft_base/public/ft_wchar.h
$(O_DIR)/_public/ft/getkey.h: libft/ft_getkey/public/getkey.h
$(O_DIR)/_public/ft/lexer.h: libft/ft_lexer/public/lexer.h
$(O_DIR)/_public/ft/libft.h: libft/ft_base/public/libft.h
$(O_DIR)/_public/ft/set.h: libft/ft_set/public/set.h
$(O_DIR)/_public/ft/spanlist.h: srcs/spanlist/public/spanlist.h
$(O_DIR)/_public/ft/str_list.h: srcs/ft_str_list/public/str_list.h
$(O_DIR)/_public/ft/strset.h: srcs/strset/public/strset.h
$(O_DIR)/_public/ft/term.h: libft/ft_term/public/term.h
$(O_DIR)/_public/ft/tokenizer.h: libft/ft_tokenizer/public/tokenizer.h
$(O_DIR)/_public/ft/tokenmap_builder.h: \
libft/ft_tokenizer/public/tokenmap_builder.h
$(O_DIR)/_public/ft/utils.h: srcs/ft_utils/public/utils.h
$(O_DIR)/_public/sh/ast.h: srcs/sh_ast/public/ast.h
$(O_DIR)/_public/sh/context.h: srcs/sh_context/public/context.h
$(O_DIR)/_public/sh/default_builtins.h: \
srcs/sh_default_builtins/public/default_builtins.h
$(O_DIR)/_public/sh/exec.h: srcs/sh_exec/public/exec.h
$(O_DIR)/_public/sh/parser.h: srcs/sh_parser/public/parser.h
$(O_DIR)/_public/sh/parser_context.h: \
srcs/sh_parser_context/public/parser_context.h
