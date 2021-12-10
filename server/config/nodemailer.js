/**
 * This is to config everything in nodemailer email
 */
//=============== MODULES IMPORT ==================
var express = require('express')
var bodyParser = require('body-parser')
const nodemailer = require('nodemailer');
const hbs = require('nodemailer-handlebars');
var dotenv = require('dotenv')

//=============== APP CONFIG ======================
var app = express()
app.use(bodyParser.urlencoded({ extended: true }))
dotenv.config({ path: __dirname + '/config/.env' });


//=============== VIEW CONFIG ======================
app.use(express.static(__dirname));

//============== MIDDLEWARES CONFIG ================
// Index page
app.get('/', (req, res) => {
    res.redirect('/index')
})

app.get('/index', (req, res) => {
    res.sendFile(__dirname + '/index.html')
})

// Contact page
app.get('/contact', (req, res) => {
    res.sendFile(__dirname + '/contact.html')
})

// Donation page 
app.get('/donation', (req, res) => {
    res.sendFile(__dirname + '/donation.html')
})

//About page
app.get('/about', (req, res) => {
    res.sendFile(__dirname + '/About.html')
})

app.post('/sendMail', (req, res) => {
    var messageBody = {
        subject: req.body.subject,
        name: req.body.name,
        email: req.body.email,
        number: req.body.number,
        content: req.body.message
    }

    const transporter = nodemailer.createTransport({
        service: 'gmail',
        auth: {
            user: process.env.EMAIL,
            pass: process.env.EMAIL_PASSWORD
        }
    });

    // Send to client a waiting email
    transporter.use('compile', hbs({
        viewEngine: {
            defaultLayout: false,
            extName: 'express-handlebars'
        },
        viewPath: './'
    }));

    // Send placeholding email to client
    let mailOptionsClient = {
        from: process.env.EMAIL,
        to: messageBody.email,
        subject: '💓 Cảm ơn vì đã giữ liên lạc với chúng tôi 💓',
        template: 'email',
        context: {
            name: messageBody.name
        } // send extra values to template
    };

    transporter.sendMail(mailOptionsClient, (err, data) => {
        var msg = undefined
        if (err) {
            console.log(err)
            msg = 'We are facing some technical difficulties here, come back later 😥'
            res.send(msg)
        }
        msg = 'OK'
        res.send(msg)
    });

    // Send email to admin side
    var mailOptions = {
        from: process.env.EMAIL,
        to: 'nhutnguyenf330@gmail.com',
        subject: 'Customer feedback incoming',
        template: 'email_admin',
        context: {
            subject: messageBody.subject,
            name: messageBody.name,
            email: messageBody.email,
            content: messageBody.content,
            number: messageBody.number
        } // send extra values to template
    };
    
    transporter.sendMail(mailOptions, function(error, info){
        if (error) {
            console.log(error);
            var message = 'We are facing some technical difficulties here, come back later 😥'
            res.send(message)
        } else {
            var message = 'OK'
            res.send(message)
        }
    });

})

// ================ 404 not found handle ===========//
app.use('/:route', (req, res) =>{
    var route = req.params.route

    res.sendFile(__dirname + '/404.html')
})

//================= SERVER LISTENER ================
var server = app.listen(process.env.PORT || 8080, () => {
    console.log('http://localhost:8080')
})
