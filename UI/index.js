const express = require('express');
const app = express();
const mongoose = require('mongoose');

const PORT = process.env.PORT || 3000;

const URI = "mongodb+srv://tropcho05:Yim9fX6SKApozmC3@Sniff.x3lngqi.mongodb.net/?retryWrites=true&w=majority/test";

mongoose.connect(URI, { useNewUrlParser: true, useUnifiedTopology: true });

mongoose.connection.on('connected', () => {
    if(mongoose.connection.readyState == 1){
        console.log('\x1b[32m%s\x1b[0m', 'MongoDb connection established');
    }else{
        console.log('\x1b[31m%s\x1b[0m', 'MongoDb connection failed!');
    }

    mongoose.connection.db.admin().command({ping: 1}).then((res) => {
        if(res.ok == 1)
            console.log('\x1b[32m%s\x1b[0m', 'Successfully pinged database');
        else
            console.log('\x1b[31m%s\x1b[0m', 'Failed to ping database');
    });
});

async function readDB(){
    const db = mongoose.connection.db;
    const collection = db.collection('sniff');
    const data = await collection.find({}).toArray();
    // console.log(data);
    return data;
}

app.get('/', (req, res) => {
    readDB().then((data) => {
        const msgs = data.map((item) => {
            return item.msg;
        });
        res.send(msgs);
    });
});

app.listen(PORT, () => {
    console.log(`Server is running on http://127.0.0.1:${PORT}`);
});

process.on('SIGINT', () => {
    mongoose.connection.close();
    mongoose.disconnect();
    console.log('\x1b[31m%s\x1b[0m', 'MongoDb connection closed due to application termination');
    process.exit(0);
});