CFLAGS	      = -O
CC            = cc
PROG	      =	gdriver
PROG2	      =	ndriver
PROG3	      =	onedriver
HDRS	      =	standn.h stand.h tokens.h patterns.h clues.h

OBJS	      =	\
		check_blank.o	\
		dict_match.o	\
		gdriver.o	\
		name_split.o	\
		read_d.o	\
		search_table.o	\
		standn.o	\
		addr_stan2.o	\
		build_input_tokens.o	\
		build_patterns.o	\
		build_stand_address.o	\
		deallocate.o		\
		decodeaddrfield.o	\
		legittokenexists.o	\
		load_cluewords.o	\
		load_master_clues.o	\
		load_patterns.o		\
		new_locations.o		\
		print_stand_fields.o	\
		putrec.o		\
		search_clue_table.o	\
		search_combo_patterns.o	\
		search_master_clues.o	\
		search_pattern_table.o	\
		squish.o		\
		stan_iden.o		\
		stan_type_decode.o	\
		tokenfound.o


OBJS2	      =	\
		check_blank.o	\
		dict_match.o	\
		ndriver.o	\
		name_split.o	\
		read_d.o	\
		search_table.o	\
		standn.o	\
		addr_stan2.o	\
		build_input_tokens.o	\
		build_patterns.o	\
		build_stand_address.o	\
		deallocate.o		\
		decodeaddrfield.o	\
		legittokenexists.o	\
		load_cluewords.o	\
		load_master_clues.o	\
		load_patterns.o		\
		new_locations.o		\
		print_stand_fields.o	\
		putrec.o		\
		search_clue_table.o	\
		search_combo_patterns.o	\
		search_master_clues.o	\
		search_pattern_table.o	\
		squish.o		\
		stan_iden.o		\
		stan_type_decode.o	\
		tokenfound.o

OBJS3	      =	\
		check_blank.o	\
		dict_match.o	\
		onedriver.o	\
		name_split.o	\
		read_d.o	\
		search_table.o	\
		standn.o	\
		addr_stan2.o	\
		build_input_tokens.o	\
		build_patterns.o	\
		build_stand_address.o	\
		deallocate.o		\
		decodeaddrfield.o	\
		legittokenexists.o	\
		load_cluewords.o	\
		load_master_clues.o	\
		load_patterns.o		\
		new_locations.o		\
		print_stand_fields.o	\
		putrec.o		\
		search_clue_table.o	\
		search_combo_patterns.o	\
		search_master_clues.o	\
		search_pattern_table.o	\
		squish.o		\
		stan_iden.o		\
		stan_type_decode.o	\
		tokenfound.o

$(PROG):	$(OBJS)
		$(CC) $(CFLAGS) -o $@ $(OBJS) -lm

$(PROG2):	$(OBJS2)
		$(CC) $(CFLAGS) -o $@ $(OBJS2) -lm

$(PROG3):	$(OBJS3)
		$(CC) $(CFLAGS) -o $@ $(OBJS3) -lm

clean:
		-rm -f $(OBJS) core a.out output2

check_blank.o:	standn.h check_blank.c
dict_match.o:	standn.h dict_match.c
gdriver.o:	standn.h	\
		stand.h		\
		gdriver.c	\
		patterns.h
ndriver.o:	standn.h	\
		stand.h		\
		ndriver.c	\
		patterns.h
name_split.o:	standn.h name_split.c
new_locations.o:	new_locations.c
read_d.o:	standn.h read_d.c
standn.o:	standn.h standn.c
search_table.o:	search_table.c
addr_stan2.o:	addr_stan2.c	\
		stand.h		\
		clues.h		\
		tokens.h	\
		patterns.h
build_input_tokens.o:	build_input_tokens.c	\
			stand.h			\
			clues.h			\
			tokens.h
build_patterns.o:	build_patterns.c	\
			stand.h			\
			tokens.h		\
			patterns.h
build_stand_address.o:	\
			build_stand_address.c	\
			stand.h			\
			clues.h			\
			tokens.h		\
			patterns.h
deallocate.o:		deallocate.c	\
			clues.h		\
			tokens.h	\
			patterns.h
decodeaddrfield.o:	decodeaddrfield.c	\
			clues.h
legittokenexists.o:	legittokenexists.c	\
			clues.h
load_cluewords.o:	load_cluewords.c	\
			clues.h
load_master_clues.o:	load_master_clues.c	\
			clues.h
load_patterns.o:	load_patterns.c		\
			patterns.h
print_stand_fields.o:	print_stand_fields.c	\
			stand.h			\
			clues.h
putrec.o:		putrec.c
search_clue_table.o:	search_clue_table.c	\
			clues.h
search_combo_patterns.o:	\
			search_combo_patterns.c	\
			stand.h			\
			tokens.h		\
			patterns.h
search_master_clues.o:	search_master_clues.c	\
			clues.h
search_pattern_table.o:	search_pattern_table.c	\
			tokens.h		\
			patterns.h
squish.o:		squish.c
stan_iden.o:		stan_iden.c
stan_type_decode.o:	stan_type_decode.c	\
			clues.h
tokenfound.o:		tokenfound.c
