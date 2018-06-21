r=30;
x1=0;
x2=0;
x3=22;
y1=0;
y2=25;
y3=12.5;

kolumna1=line([0,0],[0,0],[0,70]); 
kolumna2=line([0,0],[25,25],[0,70]);
kolumna3=line([22,22],[12.5,12.5],[0,70]); 
view(30,60)

xc=10;
yc=13;
zc=25;

z1=zc+sqrt(r^2-(x1-xc)^2-(y1-yc)^2);
z2=zc+sqrt(r^2-(x2-xc)^2-(y2-yc)^2);
z3=zc+sqrt(r^2-(x3-xc)^2-(y3-yc)^2);

ramie2=line([x2,xc],[y2,yc],[z2,zc]);
ramie1=line([x1,xc],[y1,yc],[z1,zc]);
ramie3=line([x3,xc],[y3,yc],[z3,zc]);

for t=0:0.1:6.2832
    xc=10+10*sin(t);%zakres od 9 do 11
    yc=13+10*cos(t);%zakres od 12 do 14
    zc=25;%bez zmian, bo robimy kolo poziomo
    z1=zc+sqrt(r^2-(x1-xc)^2-(y1-yc)^2);
    z2=zc+sqrt(r^2-(x2-xc)^2-(y2-yc)^2);
    z3=zc+sqrt(r^2-(x3-xc)^2-(y3-yc)^2);
    
    if(t>0)
        kroki1=round(200*(z1-stare1));
        kroki2=round(200*(z2-stare2));
        kroki3=round(200*(z3-stare3));
        [kroki1 kroki2 kroki3]
        fprintf(s, 'x %d y %d z %d\n',[kroki1, kroki2, kroki3]);
    while(s.BytesAvailable == 0)%zastepuje pause()(czeka na cokolwiek w buforze) 
    end
    while(s.BytesAvailable>0)%czyta wszystko z bufora
        fscanf(s);
    end
    
    end
    drawnow
    ramie1.XData=[x1 xc];
    ramie1.YData=[y1 yc];
    ramie1.ZData=[z1 zc];
    ramie2.XData=[x2 xc];
    ramie2.YData=[y2 yc];
    ramie2.ZData=[z2 zc];
    ramie3.XData=[x3 xc];
    ramie3.YData=[y3 yc];
    ramie3.ZData=[z3 zc];
    
    stare1=z1;
    stare2=z2;
    stare3=z3;
    
end