function Is = read_imgs(homedir,imgdir,prename,postname,digits,startid,endid,step_img)
%
% Is = read_imgs(homedir,imgdir,prename,postname,digits,startid,endid,step_img)
%



command = ['%s%s%s%.',num2str(digits),'d%s'];

fname = sprintf(command,homedir,imgdir,prename,startid,postname);
disp(fname);
if (strcmp('.pgm',postname)),
 I1 = readpgm(fname);
elseif (strcmp('.ppm',postname))
 a = readppm(fname);
 I1 = sum(a,3);
else
   a = imread(fname); a = sum(double(a),3);
   I1 = a;
end


Is = zeros(size(I1,1),size(I1,2),1+floor((endid-startid)/step_img));
Is(:,:,1) = I1;
im_id = 1;
for j = startid+step_img:step_img:endid,
    command = ['%s%s%s%.',num2str(digits),'d%s'];
    fname = sprintf(command,homedir,imgdir,prename,j,postname);
    disp(fname);
    im_id = im_id+1;
    
    if (strcmp('.pgm',postname)),
      Is(:,:,im_id) = readpgm(fname);
    elseif (strcmp('.ppm',postname))
      a = readppm(fname);
      Is(:,:,im_id) = sum(a,3);
    else
      a = imread(fname); a = sum(double(a),3);
      Is(:,:,im_id) = a;
    end
end






