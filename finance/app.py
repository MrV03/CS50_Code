import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    portfolio = db.execute(
        "SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0",
        user_id,
    )

    stocks = []
    total_value = cash

    for stock in portfolio:
        symbol = stock["symbol"]
        shares = stock["total_shares"]
        stock_info = lookup(symbol)
        price = stock_info["price"]
        value = price * shares
        total_value += value
        stocks.append(
            {
                "symbol": symbol,
                "name": stock_info["name"],
                "shares": shares,
                "price": usd(price),
                "value": usd(value),
            }
        )

    return render_template(
        "index.html", stocks=stocks, cash=usd(cash), total_value=usd(total_value)
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol or not shares:
            return apology("must provide stock symbol and number of shares", 400)

        try:
            shares = int(shares)
            if shares <= 0:
                raise ValueError("number of shares must be a positive integer")
        except ValueError:
            return apology("invalid number of shares", 400)

        stock = lookup(symbol)
        if not stock:
            return apology("invalid stock symbol", 400)

        cost = stock["price"] * shares
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0][
            "cash"
        ]

        if cash < cost:
            flash("Not enough cash to buy the specified number of shares")
            return redirect("/")

        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, timestamp) VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)",
            session["user_id"],
            stock["symbol"],
            shares,
            stock["price"],
        )

        db.execute(
            "UPDATE users SET cash = cash - ? WHERE id = ?", cost, session["user_id"]
        )

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]

    transactions = db.execute(
        "SELECT CASE WHEN shares > 0 THEN 'BUY' ELSE 'SELL' END as type, symbol, shares, price, timestamp FROM transactions WHERE user_id = ? ORDER BY timestamp DESC",
        user_id,
    )

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide stock symbol", 400)

        stock = lookup(symbol)
        if not stock:
            return apology("invalid stock symbol", 400)

        return render_template("quote.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check for empty fields
        if not username or not password or not confirmation:
            return apology("must provide username, password, and confirmation", 400)

        # Check if password and confirmation match
        if password != confirmation:
            return apology("passwords must match", 400)

        # Check if username already exists
        existing_user = db.execute("SELECT * FROM users WHERE username = ?", username)
        if existing_user:
            return apology("username already exists", 400)

        # Hash the password and insert the new user into the database
        hashed_password = generate_password_hash(password)
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            username,
            hashed_password,
        )

        # Log in the user automatically after registration
        user_id = db.execute("SELECT id FROM users WHERE username = ?", username)[0][
            "id"
        ]
        session["user_id"] = user_id

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol or not shares:
            return apology("must provide stock symbol and number of shares", 400)

        try:
            shares = int(shares)
            if shares <= 0:
                raise ValueError("number of shares must be a positive integer")
        except ValueError:
            return apology("invalid number of shares", 403)

        user_id = session["user_id"]
        user_shares = db.execute(
            "SELECT SUM(shares) as total_shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol",
            user_id,
            symbol,
        )

        if not user_shares or user_shares[0]["total_shares"] < shares:
            return apology("not enough shares to sell", 400)

        stock = lookup(symbol)
        if not stock:
            return apology("invalid stock symbol", 400)

        value = stock["price"] * shares

        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, timestamp) VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)",
            user_id,
            stock["symbol"],
            -shares,
            stock["price"],
        )

        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", value, user_id)

        flash("Sold successfully!")

        return redirect("/")

    else:
        user_id = session["user_id"]
        portfolio = db.execute(
            "SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0",
            user_id,
        )

        return render_template("sell.html", portfolio=portfolio)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change user password"""
    if request.method == "POST":
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        if not current_password or not new_password or not confirmation:
            return apology(
                "must provide current password, new password, and confirmation", 403
            )

        user_id = session["user_id"]
        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

        if not check_password_hash(user["hash"], current_password):
            flash("Invalid current password")
            return redirect("/change_password")

        if new_password != confirmation:
            return apology("new passwords must match", 403)

        hashed_password = generate_password_hash(new_password)

        db.execute("UPDATE users SET hash = ? WHERE id = ?", hashed_password, user_id)

        return redirect("/")

    else:
        return render_template("change_password.html")
