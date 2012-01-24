filetype plugin indent on

au FileType ruby setl ts=2 sw=2 tw=120 expandtab nowrap
au FileType c,cpp,h,hpp,hh,cc setl ts=8 sw=8 tw=120 noexpandtab
au FileType cucumber setl sw=2 ts=2 sts=2 expandtab nowrap
au FileType html,javascript setl ts=2 sw=2 tw=120 expandtab nowrap
au FileType sh,spec setl ts=4 sw=4 tw=120 expandtab wrap cindent

set pastetoggle=<F2>
set wmh=0

let html_use_css=1
let html_number_lines=1
let html_no_pre=1

iab INDOC #####<CR># NAME:<CR>#<CR># SYNOPSIS:<CR>#<CR># DESCRIPTION:<CR>#<CR># RETURN VALUE:<CR>#
