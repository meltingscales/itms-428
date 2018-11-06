use itms428;

#This command should fail, because this user does not have privileges to drop
Drop Table employees;

#This command should fail, because this user does not have this privilege to alter
Alter Table employees
Add lastName Varchar(50);

#This test should pass because this user has select privileges
Select * From offices;

#This command failed because this user does not have the privilege to do this execution
Update offices
Set city = 'Schaumburg'
Where officeCode = '1';

#Testing passed for This command because this user has privilege to update
SET SQL_SAFE_UPDATES = 0;
update customers
Set customerNumber = 102
Where customerName = 'Atelier graphique';

#This command failed because this user does not have privileges to do that
Alter Table customers
Add contactLastname Varchar(50);