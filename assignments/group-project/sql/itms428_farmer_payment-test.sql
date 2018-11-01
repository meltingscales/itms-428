Use itms428;

Select id From farmer_payment;



#Update works, since this user has update privilege given
UPDATE farmer_payment
SET n3 = 100 WHERE id=1;


#Insert fail, because this user does not have the permisssion
Insert into farment_payment values (NULL, 3, 54, "rose", 8,9,52,NOW(), cook, 5, 2005,2009,4);


#Deleting row is not allowed for farmer_payment
#It does not have privilegae to do that
Delete from farmer_payment
where id =1;

