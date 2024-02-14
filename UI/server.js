const express = require('express')
const app = express()
const fs = require('fs')
const { MongoClient, ServerApiVersion } = require('mongodb');
const bcrypt = require('bcrypt')
const Validator = require('validator')
app.use(express.urlencoded({ extended: true }));
const nodemailer = require('nodemailer');
const cookieParser = require('cookie-parser');
const { atob } = require('buffer');
app.use(cookieParser());
let email1;
let hash1;
let auth_code;
app.set("view engine", "ejs");

const PORT = process.env.PORT || 3000;

let lastUpdate = Date.now();

// Endpoint to check for updates
app.get('/check-for-updates', (req, res) => {
    const clientTimestamp = Number(req.query.timestamp);
    if (isNaN(clientTimestamp)) {
        res.status(400).send({ error: 'Bad Request', message: 'Timestamp is required' });
    } else {
        res.send({ hasUpdate: lastUpdate > clientTimestamp });
    }
});

//Connect to DB
const client = new MongoClient("mongodb+srv://tropcho05:Yim9fX6SKApozmC3@Sniff.x3lngqi.mongodb.net/?retryWrites=true&w=majority", { useNewUrlParser: true, useUnifiedTopology: true, serverApi: ServerApiVersion.v1 });
client.connect()

const LoginCollection = client.db("Users").collection("Login");
const OBDCollection = client.db("test").collection("OBD");
const GPSCollection = client.db("test").collection("GPS");

function convertToWGS84(lat, lon) {
    // Extracting degrees, minutes, and seconds for latitude
    const latDeg = parseInt(lat.substring(0, 2));
    const latMin = parseFloat(lat.substring(2));

    // Extracting degrees, minutes, and seconds for longitude
    const lonDeg = parseInt(lon.substring(0, 2));
    const lonMin = parseFloat(lon.substring(2));

    // Converting latitude and longitude to decimal degrees
    const latitude = latDeg + (latMin / 60);
    const longitude = lonDeg + (lonMin / 60);

    // Returning latitude and longitude in WGS84 format
    return {
        latitude: latitude,
        longitude: longitude
    };
}

function convertHMToWGS84(longitude, latitude) {
    return proj4('H:M', 'WGS84', [longitude, latitude]);
}

app.get("/contactsPage", async(req, res) => {
    res.render("contactsPage")
})



//accountPage
app.get("/accountPage", async(req, res) => {
    try
    {
        let cookie = req.cookies.Login.token
        if(cookie)
        {
            let FindToken = await LoginCollection.findOne({token:cookie})
            if(FindToken)
            {
                const d = new Date()
                let date = d.getTime()
                if(date < FindToken.expire)
                {
                    await LoginCollection.updateOne({token:cookie}, {$set:{expire:generateDate()}})
                    res.render("LoggedInAccPage")
                }
                else{
                    res.render("accountPage", {login_error: "", register_error: ""})
                }
            }
            else
            {
                res.render("accountPage", {login_error: "", register_error: ""})
            }
        }
    }
    catch{
        res.render("accountPage", {login_error: "", register_error: ""})
    }

})


app.get("/LoggedInAccPage", (req, res) => {
    res.redirect("/accountPage")
})

app.get("/trackersPage", async(req, res) => {
    try
    {
        let cookie = req.cookies.Login.token
        if(cookie)
        {
            let FindToken = await LoginCollection.findOne({token:cookie})
            if(FindToken)
            {
                const d = new Date()
                let date = d.getTime()
                if(date < FindToken.expire)
                {
                    if(FindToken.id)
                    {
                        // fetch data from GPS_COLLECTION
                        let data = await GPSCollection.find({id:FindToken.id}).toArray()
                        let json = JSON.stringify(data)
                        json = JSON.parse(json)
                        let payload = json[json.length-1].payload
                        console.log("Payload: " + payload)
                        let payload_d;
                        try {
                        const decodedPayload = Buffer.from(payload, 'base64').toString('hex');
                        payload_d = decodedPayload;
                        console.log('Decoded payload:' + payload_d);
                        } catch (error) {
                        console.error('An error occurred while decoding the payload:', error);
                        }

                        let firstTwoChars = payload_d.slice(0, 4) + '.' + payload_d.slice(4, 8);
                        let restOfString = payload_d.slice(8, 12) + '.' + payload_d.slice(12);

                        console.log("First two chars: " + firstTwoChars + "\n")
                        console.log("Rest of string: " + restOfString + "\n")

                        // let coords = firstTwoChars + ',' + restOfString;

                        const coords = convertToWGS84(firstTwoChars, restOfString);

                        console.log("Coords: " + coords.latitude + ", " + coords.longitude + "\n")

                        // convert milliseconds since the Unix epoch to date and time
                        // sample: 1707563000209
                        let date = new Date(Number(json[json.length-1].reported_at));

                        lastUpdate = date;

                        // console.log("Raw date: " + payload.reported_at + "\n")
                        console.log("Reported at: " + date + " by " + FindToken.id + "\n")

                        await res.render("trackersPage", {coords: (coords.latitude + "," + coords.longitude), reported_at: date})
                        await LoginCollection.updateOne({token:cookie}, {$set:{expire:generateDate()}})
                    }
                    else
                    {
                        await res.redirect("/addDevicePage")
                    }
                }
                else
                {
                    res.render("accountPage", {login_error: "", register_error: ""})
                }
            }
            else
            {
                res.render("accountPage", {login_error: "", register_error: ""})
            }
        }
    }
    catch(err){
        console.error(err)
        res.render("accountPage", {login_error: "", register_error: ""})
    }

})


app.get("/", (req, res) => {
    res.render("homePage")
})


app.get("/addDevicePage", async(req, res) => {
    try
    {
        let cookie = req.cookies.Login.token
        if(cookie)
        {
            let FindToken = await LoginCollection.findOne({token:cookie})
            if(FindToken)
            {
                const d = new Date()
                let date = d.getTime()
                if(date < FindToken.expire)
                {
                    await LoginCollection.updateOne({token:cookie}, {$set:{expire:generateDate()}})
                    res.render("addDevicePage")
                }
                else{
                    res.render("accountPage", {login_error: "", register_error: ""})
                }
            }
            else
            {
                res.render("accountPage", {login_error: "", register_error: ""})
            }
        }
    }
    catch{
        res.render("accountPage", {login_error: "", register_error: ""})
    }
})

app.get("*", async (req,res) => {
    let url = req.url
    try{
        await res.sendFile(__dirname + "/views/" + url);
    }
    catch
    {
        res.end("Not Found")
    }
})


//LogOut
app.post("/logout", (req, res) => {
    res.clearCookie("Login")
    res.redirect("/accountPage")
})

//Register
app.post("/register", async (req, res) => {
    let data = req.body
    if(data.password != data.repeat_password)
    {
        res.render("accountPage", {login_error: "", register_error: "Passwords don't match"})
    }
    else if(Validator.isEmail(data.email) == false)
    {
        res.render("accountPage", {login_error: "", register_error: "The email is incorrect"})
    }
    else if(await LoginCollection.findOne({email: data.email}))
    {
        res.render("accountPage", {login_error: "", register_error: "An account with this email already exists"})
    }
    else
    {
        email1 = data.email
        hash1 = await bcrypt.hash(data.password, 10)
        email_authorization()
        res.render("accVerifPage", {error:""})
        
    }

});

async function email_authorization()
{
    auth_code = Math.floor(100000 + Math.random() * 900000)
    let transporter = nodemailer.createTransport({
        host: 'smtp.gmail.com',
        port: 465,
        secure: true,
        auth: {
            user: 'petsafe37@gmail.com',
            pass: 'qqyokzrqpyelygke'
        }
    });

    let mailOptions = {
    from: 'PetSafe2FA',
    to: email1,
    subject: 'Authorization Code',
    text: auth_code.toString()
    };

    transporter.sendMail(mailOptions, function(error, info){
    if (error) {
        console.log(error);
    } else {
        console.log('Email sent: ' + info.response);
    }
    });
}

//Generate Token
const characters ='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-.@_/';

function generateString(length) {
    let result = '';
    const charactersLength = characters.length;
    for ( let i = 0; i < length; i++ ) {
        result += characters.charAt(Math.floor(Math.random() * charactersLength));
    }

    return result;
}

//generate expiration date
function generateDate()
{
    const d = new Date();
    const newDate = new Date(d.getTime() + 30*60000)
    return newDate
}

//Auth Page

app.post("/authorization", async(req, res) => {
    if(req.body.auth_code == auth_code)
    {
        let token = generateString(30);
        let expire = generateDate();
        await LoginCollection.insertOne({email:email1, password: hash1, token:token, expire:expire, id: null})
        res.cookie("Login", {token: token})
        res.redirect("/accountPage");
    }
    else
    {
        res.render("accVerifPage", {error:"The verification code is incorrect"})
    }
})

//Login
app.post("/login", async (req, res) => {
    let data = req.body;
    if(Validator.isEmail(data.email) == false)
    {
        res.render("accountPage", {login_error: "The email is invalid", register_error: ""})
    }
    else if(await LoginCollection.findOne({email: data.email}))
    {
        let user = await LoginCollection.findOne({email: data.email});
        let iscorrect = await bcrypt.compare(data.password, user.password);
        if(iscorrect)
        {
            let token = generateString(30)
            let expire = generateDate()
            await LoginCollection.updateOne({email:data.email}, {$set:{token: token, expire:expire}})
            res.cookie("Login", {token: token})
            res.redirect("/accountPage");
        }
        else
        {
            res.render("accountPage", {login_error: "The credentials are incorrect", register_error: ""})  
        }
    }
    else
    {
        res.render("accountPage", {login_error: "The credentials are incorrect", register_error: ""}) 
    }
})

//add api key

app.post("/api_key", async(req, res) => {
    let data = req.body
    let cookie = req.cookies.Login.token
    await LoginCollection.updateOne({token:cookie}, {$set:{id:data.api_key}})
    res.redirect("/trackersPage")
})

app.listen(PORT, () => {
    console.log(`Server is running on http://127.0.0.1:${PORT}`);
});

process.on('SIGINT', () => {
    client.close();
    console.log('\x1b[31m%s\x1b[0m', 'MongoDb connection closed due to application termination');
    process.exit(0);
});
