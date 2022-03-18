%define label 0
%macro colon 2
%ifid %2
    %%nextlabel: dq label
%else
    %fatal "ERROR: label identifier expected"
%endif
%ifstr %1
    db %1, 0
    %2:
%else
    %fatal "ERROR: string expected as a key value"
%endif
%define label %%nextlabel
%endmacro