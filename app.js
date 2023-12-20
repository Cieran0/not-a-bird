const express = require('express');
const session = require('express-session');
const fs = require('fs');
const app = express();
const {
    execSync
} = require('child_process');

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

app.use(express.json());

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


// Routes
app.get('/', requireLogin, (req, res) => {
    const username = req.session.username; // Get the username from the session or set a default value
    // Read the index.html file and replace $NAME with the actual username
    fs.readFile(__dirname + '/public/index.html', 'utf8', (err, data) => {
        if (err) {
            console.error('Error reading file:', err);
            res.status(500).send('Internal Server Error');
        } else {
            updatedHTML = data.replace('$NAME', username);
            posts = getAllPosts(req.session.username);
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
        req.session.password = password;
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
        req.session.password = password;
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

app.post('/likePost', (req, res) => {
    const {
        postID
    } = req.body;
    res.send("Liked: " + postID);
    likePost(req.session.username, req.session.password, postID);
});


// Redirect to login if the route doesn't match any defined routes
app.use((req, res) => {
    res.redirect('/login');
});

app.listen(3000, () => {
    console.log('Server is running on port 3000');
});

function makeIntoPost(content, author, postID, liked) {
    likedString = liked ? "liked" : "";
    const postFooter = `<div class=\"postFoot\">
    <button type="button" class="likeButton $" onclick=\"toggleLike(this)\">Like</button>
    </div>`.replace('$', likedString);
    return "<div class=\"post\" id=\"" + postID + "\"><div class=\"postHead\">" + author + "</div>\n" + content.replaceAll("\n", "<br>") + postFooter + "</div>\n"
}

function getAllPosts(username, password) {
    const posts = [];
    postIDs = runCommandAndGetOutput("db/db get-all-posts").split('\n');
    likedPosts = getLikedPosts(username, password);
    for (let i = 0; i < postIDs.length; i++) {
        if (postIDs[i] == "")
            continue;
        postInfo = runCommandAndGetOutput("db/db get-post " + postIDs[i]);
        postInfoSplit = postInfo.split('\n');
        postAuthorID = postInfoSplit[1];
        postTime = postInfoSplit[2];
        postLikeCount = postInfoSplit[3];
        postContent = "";
        for (let i = 4; i < postInfoSplit.length; i++) {
            postContent += postInfoSplit[i] + "\n";
        }
        liked = false;
        for (let j = 0; j < likedPosts.length; j++) {
            if (postIDs[i] == likedPosts[j]) {
                liked = true;
                break;
            }
        }
        posts.push(makeIntoPost(postContent, getUsername(postAuthorID), postIDs[i], liked));
    }
    return posts;
}

function newPost(postContent) {
    console.log(runCommandAndGetOutput("db/db add-post cieran epic $\'" + postContent.replace('\r,\n') + "\'"));
}

function runCommandAndGetOutput(command) {
    try {
        const output = execSync(command, {
            encoding: 'utf-8'
        });
        return output;
    } catch (error) {
        return `Error executing the command: ${error.message}`;
    }
}

const usernameCache = {};

function getUsername(userID) {
    const key = userID;
    if (!usernameCache[key]) {
        usernameCache[key] = runCommandAndGetOutput("db/db get-username " + userID);
    }
    return usernameCache[key];
}

function likePost(username, password, postID) {
    output = runCommandAndGetOutput("db/db toggle-like-post " + username + " " + password + " " + postID);
}

function getLikedPosts(username) {
    output = runCommandAndGetOutput("db/db get-user " + username);
    userData = output.split('\n');
    likedPosts = [];
    for (let i = 3; i < userData.length; i++) {
        likedPosts.push(userData[i])
    }
    return likedPosts;
}