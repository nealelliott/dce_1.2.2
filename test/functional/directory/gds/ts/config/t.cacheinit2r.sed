:*****TEST  (INITIALIZE CACHE) ****:
:*** sccsid = @(#)t.cacheinit2	1.4 91/09/09 (K Sys Ap 11) ****:
:directory id:2
:Authentication mechanism to be used:Anonymous
:options():Logon to the DUA Cache
:****Administration ****:
:Function:1
:****Add Client Address *******:
:Operation:05
:P-Selector:
:S-Selector:
:T-Selector:Client
:Net-address 1:TCP/IP!internet=CLIENT_IP+port=21010
:Net-address 2:
:Net-address 3:
:Net-address 4:
:Net-address 5:
:****AddObject  de/dbp/dap11/dsa/dsa-m2 *******:
:Operation:01
:Objekttypnummer:07
:country:de
:organization:dbp
:OrganizationalUnit:dap11
:Common-Name:dsa
:Common-Name:dsa-m2
:ObjectClass:Directory-Service-Agent
:Auxiliary Object Class:NO
:Attribute Name1:Presentation-Address
:Attribute Name2:DSA-Type
:Attribute Name3:
:Attribute Name4:
:Attribute Name5:
:More:
:attributName:DSA-Type
:attributwert:default/local'
:attributwert:
:attributName:
:attributwert:
:attributwert:
:attributName:
:attributwert:
:attributwert:
:P-Selector:
:S-Selector:
:T-Selector:Server
:Net-address 1:TCP/IP!internet=SERVER_IP+port=21012
:Net-address 2:
:Net-address 3:
:Net-address 4:
:Net-address 5:
:****AddObject  de/dbp/dap11/dsa/dsa-m1 *******:
:Operation:01
:Objekttypnummer:07
:country:de
:organization:dbp
:OrganizationalUnit:dap11
:Common-Name:dsa
:Common-Name:dsa-m1
:ObjectClass:Directory-Service-Agent
:Auxiliary Object Class:NO
:Attribute Name1:Presentation-Address
:Attribute Name2:
:Attribute Name3:
:Attribute Name4:
:Attribute Name5:
:More:
:P-Selector:
:S-Selector:
:T-Selector:Server
:Net-address 1:TCP/IP!internet=SERVER_IP+port=21011
:Net-address 2:
:Net-address 3:
:Net-address 4:
:Net-address 5:
:******** RETURN ******:
:operation:00
:**** Administration ****:
:Function:0
