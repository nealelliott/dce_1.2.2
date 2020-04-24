:*************************************************************************:
:****  Copy Subtree Batchfile. 					     *****:
:****                                                                *****:
:*************************************************************************:
:**** INITIALIZATION BATCHFILE *****:
:directory id:11
:Authentication mechanism to be used:Anonymous
:options() Logon to DUA Cache :Logon to the Default DSA
:****Administration ****:
:Function Subtree Administration:4
:****Copy Subtree *******:
:Operation:3
:Source Objekttypnummer:02
:Object interpretation:OBJECT AND ITS SUBORDINATES
:country:re
:Source DSA:Master DSA(s)
:Target Object number:00
:Overwrite existing Entries:YES
:New entries protected by:Original ACL
:Target DSA:SPECIFIC DSA
:country name:re
:organization:sni
:OrganizationalUnit:buba
:Common-Name:dsa
:Common-Name:dsa2
:******** RETURN ******:
:operation:0
:**** Administration ****:
:Function:0
