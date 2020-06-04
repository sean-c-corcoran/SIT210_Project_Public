# SIT210_Project_Public

This repository was developed while studying Unit SIT210 Embedded Systems Development at Deakin University. 

The repository houses the code used for my unit project called Smart Pots, To which I developed a series of scalable house plant monitoring pots using Particle Photons and a Raspberry Pi. 

To achieve the scalability the Raspberry Pi utilises a LAMP stack to store information about the smart ports (Particle Photon’s) in a MySQL database. 
To which a webpage is used to query a database and dynamically display information about different factors of each smart pot such as soil moisture and the amount light the plant receives. 

The website also lets users add and remove additional smart pots as required.

Smart pots (Particle Photons) are designed to use the same software code as they act as an end point feeding information to the Particle Cloud to be queried individually using their device ID. 

Additionally the Raspberry Pi also reads the humidity and temperature of the given environment (inside the house) to be passed to the service “ThingSpeak” which the webpage displays information widgets from the generated iframes.

Furthermore the Raspberry Pi monitors a publish event from the Particle Cloud if a smart pot’s soil moisture falls below a certain point to which it will look up it’s end user set name from the database and pass the information to the service IFTTT in the form of a Webhook.

I have created a user manual on how to run the above code snippets and build this exact system which can be downloaded from here: https://drive.google.com/file/d/16Hw_WFQY2tQBXkMJqxQEoYm29X8ap_cF/view?usp=sharing
