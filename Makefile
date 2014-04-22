

# Build corpus finder
default:
	@g++ -o corpus main.cpp

# Show makefile usage
help:
	@awk 'BEGIN{print "\nMakefile usage:\n"};/^[^#[:space:]\.].*:/&&$$0!~/=/{split($$0,t,":");printf("%8s %-16s %s\n","make",t[1],x);x=""};/^#/{gsub(/^# /,"");x=$$0;if(x!="")x="- "x};END{printf "\n"}' Makefile

.PHONY: \
	default
