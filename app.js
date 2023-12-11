const express = require('express');
const session = require('express-session');
const fs = require('fs');
const app = express();

users = [{
        username: "cieran",
        password: "epic"
    },
    {
        username: "user1",
        password: "password1"
    }
];

app.use(express.urlencoded({
    extended: true
}));

// Session middleware configuration
app.use(session({
    secret: 'your-secret-key',
    resave: false,
    saveUninitialized: true
}));

// Middleware to check if user is logged in
const requireLogin = (req, res, next) => {
    if (!req.session || !req.session.loggedIn) {
        if (req.url !== '/login') { // Avoid infinite redirect loop
            res.redirect('/login');
        } else {
            next();
        }
    } else {
        next();
    }
};

// Serve static files (HTML, CSS, JS)
//app.use(express.static('public'));

// Routes
app.get('/', requireLogin, (req, res) => {
    const username = req.session.username || 'User'; // Get the username from the session or set a default value
    // Read the index.html file and replace $NAME with the actual username
    fs.readFile(__dirname + '/public/index.html', 'utf8', (err, data) => {
        if (err) {
            console.error('Error reading file:', err);
            res.status(500).send('Internal Server Error');
        } else {
            updatedHTML = data.replace('$NAME', username);
            posts = getAllPosts(__dirname + '/private/');
            postsHTML = "";
            for (let i = 0; i < posts.length; i++) {
                postsHTML += posts[i];
            }
            updatedHTML = updatedHTML.replace('$POSTS', postsHTML);
            res.send(updatedHTML);
        }
    });
});

app.get('/login', (req, res) => {
    res.sendFile(__dirname + '/public/login.html');
});

app.get('/style.css', (req, res) => {
    res.sendFile(__dirname + '/public/style.css');
});

app.post('/login', (req, res) => {
    const {
        username,
        password
    } = req.body;
    // Dummy authentication (replace this with your authentication logic)
    foundUser = false;
    for (let i = 0; i < users.length; i++) {
        if (users[i].username == username && users[i].password == password) {
            foundUser = true;
            break;
        }
    }
    if (foundUser != "") {
        req.session.loggedIn = true;
        req.session.username = username;
        res.redirect('/');
    } else {
        res.send('Invalid username or password');
    }
});

app.get('/signup', (req, res) => {
    res.sendFile(__dirname + '/public/signup.html');
});

app.post('/signup', (req, res) => {
    const {
        username,
        password
    } = req.body;
    // This is a simple example without a database. You should store this information securely in a database.
    // In a real application, use a secure password hashing technique.

    // For demo purposes, create a user object.
    const newUser = {
        username,
        password
    };
    foundUser = false;
    for (let i = 0; i < users.length; i++) {
        if (users[i].username == username) {
            foundUser = true;
            break;
        }
    }
    if (!foundUser) {
        // Here, you might want to save the new user data to a database.
        // For simplicity, we'll just display the user data.
        // Redirect to the login page after signup (you can modify this behavior)
        req.session.loggedIn = true;
        req.session.username = username;
        users.push(newUser);
        console.log(users);
        res.redirect('/');
    } else {
        res.send('Username already in use');
    }
});

app.post('/logout', (req, res) => {
    // Clear the session and log the user out
    req.session.destroy(err => {
        if (err) {
            console.error('Error destroying session:', err);
            return res.redirect('/');
        }
        res.redirect('/login'); // Redirect to the login page after logout
    });
});

app.post('/newPost', (req, res) => {
    // Clear the session and log the user out
    const {
        postContent
    } = req.body;
    newPost(postContent);
    res.redirect('/');
});

// Redirect to login if the route doesn't match any defined routes
app.use((req, res) => {
    res.redirect('/login');
});

app.listen(3000, () => {
    console.log('Server is running on port 3000');
});

function makeIntoPost(post, author, postID) {
    const postFooter = `<div class=\"postFoot\">
    <button type="button" class="likeButton" onclick=\"toggleLike(this)\">Like</button>
    </div>`
    return "<div class=\"post\" id=\"" + postID + "\"><div class=\"postHead\">" + author + "</div>\n" + post + postFooter + "</div>\n"
}

function getAllPosts(directory) {
    const posts = [];
    const fileNames = fs.readdirSync(directory);
    i = 0;
    fileNames.forEach(fileName => {
        try {
            const data = fs.readFileSync(directory + fileName, 'utf8');
            posts.push(makeIntoPost(data, "You", i));
            i++;
        } catch (err) {
            console.error('Error reading file:', err);
        }
    });

    console.log(posts);
    return posts;
}

function newPost(postContent) {
    try {
        fs.writeFileSync(__dirname + '/private/post_' + Math.floor(Math.random() * 10000) + '.txt', postContent);
        // file written successfully
    } catch (err) {
        console.error(err);
    }
}