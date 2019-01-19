const express = require('express')
const mysql = require('mysql')
const bodyParser = require('body-parser')
const path  = require('path')
const app = express()
const port = 3000;
const {getHomePage} = require('./routes/home');
const player = require('./routes/player');
var mkdirp = require('mkdirp');
    

//configure middleware

const db = mysql.createConnection({
    //properties
    host:'localhost',
    user: 'root',
    password:'',
    database: 'northvista'
});

db.connect(function(err){
    //callback
    if(err){
        console.log('Error')
    }
    else{
        console.log('datbase conneced')
    }
    
})
global.db = db
app.set('port',process.env.port || port)
app.set('views',path.join(__dirname,'views'))
app.set('view engine', 'ejs')
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json()); // parse form data client


//app.get('/',function(req,res){
//    connect.query("SELECT * FROM users WHERE ID = 1",function(error,rows,fields){
//        if(error){
//            console.log(error)
//        }
//        else{
//            console.log(rows[0].Name)
//        }
//    })
//})
app.use('/',player)
app.use(express.static(path.join(__dirname, 'public'))); 
//app.get('/showplayer',showPlayer)
//app.get('/add',showAddPlayer)
//app.post('/add',AddPlayer)
app.listen(port, () => {
    console.log(`Server running on port: ${port}`);
});