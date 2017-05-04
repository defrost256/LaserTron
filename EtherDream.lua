-- trivial protocol example
-- declare our protocol (COMMENT IF MODIFYING)
ether_dream_proto = Proto("etherdream","EtherDream Protocol")
-- create a function to dissect it
function ether_dream_proto.dissector(buffer,pinfo,tree)
    pinfo.cols.protocol = "ETHERDREAM"
    local firstByte = buffer(0,1):string()
    local len = buffer:len()
    if firstByte == "a" or firstByte == "I" then
        local subtree = tree:add(ether_dream_proto,buffer(),"EtherDream Response")
        subtree:add(buffer(0,1), "Response "..buffer(0,1):string())
        subtree:add(buffer(1,1), "Command "..buffer(1,1):string())
        subtree:add(buffer(2,1), "Protocol", buffer(2,1):uint())
        subtree:add(buffer(3,1), "LightEngineState", buffer(3,1):uint())
        subtree:add(buffer(4,1), "PlaybackState", buffer(4,1):uint())
        subtree:add(buffer(5,1), "Source", buffer(5,1):uint())
        subtree:add(buffer(6,2), "LightEngineFlags", buffer(6,2):uint())
        subtree:add(buffer(8,2), "PlaybackFlags", buffer(8,2):uint())
        subtree:add(buffer(10,2), "SourceFlags", buffer(10,2):le_uint())
        subtree:add(buffer(12,2), "BufferFullness", buffer(12,2):le_uint())
        subtree:add(buffer(14,4), "PointRate", buffer(14,4):le_uint())
        subtree:add(buffer(18,4), "PointCount", buffer(18,4):le_uint())
    elseif firstByte == "q" then
        local subtree = tree:add(ether_dream_proto, buffer, "EtherDream Command RateChange")
        subtree:add(buffer(0,1), "Command "..buffer(0,1):string())
        subtree:add(buffer(1,4), "New Rate "..buffer(1,4):uint())
        if len > 5 then
            local pointCount = (len - 8) / 18
            subtree = tree:add(ether_dream_proto, buffer, "EtherDream Command Data")
            subtree:add(buffer(5,1), "Command "..buffer(5,1):string())
            subtree:add(buffer(6,2), "PointCount "..buffer(6,2):le_uint())
            for i=0,pointCount - 1 do
                local subsubtree = subtree:add(ether_dream_proto, buffer, "Point")
                subsubtree:add(buffer(8 + i * 18, 2), "Control "..buffer(8 + i * 18, 2):uint())
                subsubtree:add(buffer(10 + i * 18, 2), "X "..buffer(10 + i * 18, 2):int())
                subsubtree:add(buffer(12 + i * 18, 2), "Y "..buffer(12 + i * 18, 2):int())
                subsubtree:add(buffer(14 + i * 18, 2), "i "..buffer(14 + i * 18, 2):uint())
                subsubtree:add(buffer(16 + i * 18, 2), "R "..buffer(16 + i * 18, 2):uint())
                subsubtree:add(buffer(18 + i * 18, 2), "G "..buffer(18 + i * 18, 2):uint())
                subsubtree:add(buffer(20 + i * 18, 2), "B "..buffer(20 + i * 18, 2):uint())
                subsubtree:add(buffer(22 + i * 18, 2), "U1 "..buffer(22 + i * 18, 2):uint())
                subsubtree:add(buffer(24 + i * 18, 2), "U2 "..buffer(24 + i * 18, 2):uint())
            end
        end
    else
    end
end
-- load the tcp.port table
tcp_table = DissectorTable.get("tcp.port")
-- register our protocol to handle tcp port 7765
tcp_table:add(7765,ether_dream_proto)
