filetype plugin indent on

au FileType ruby,erb setl ts=2 sw=2 tw=120 expandtab nowrap
au FileType c,cpp,h,hpp,hh,cc setl ts=8 sw=8 tw=120 noexpandtab
au FileType cucumber setl sw=2 ts=2 sts=2 expandtab nowrap
au FileType html,javascript setl ts=2 sw=2 tw=120 expandtab nowrap
au FileType sh,spec setl ts=4 sw=4 tw=120 expandtab wrap cindent

set pastetoggle=<F2>
set wmh=0
set hidden

let html_use_css=1
let html_number_lines=1
let html_no_pre=1

""""
" C Abbreviations to ease coding. :-)

"""
" Include helpers

""
" Include standard libraries.
iab #include<stdio> #include <stdio.h><CR>#include <stdlib.h><CR>#include <string.h>

""
" Include socket libraries.
iab #include<socket> #include <sys/socket.h><CR>#include <sys/types.h><CR>#include <sys/select.h><CR>#include <netinet/in.h><CR>#include <arpa/inet.h><CR>#include <netdb.h>

" normal main
iab intmain int main(int argc, char *argv[])<CR>{<CR><Tab>
